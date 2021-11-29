#include "../net/TcpConnection.h"

#include "../base/Logging.h"
#include "../base/WeakCallback.h"

#include "../net/EventLoop.h"
#include "../net/Channel.h"
#include "../net/EventLoop.h"
#include "../net/Socket.h"
#include "../net/SocketsOps.h"

using namespace netLib;
using namespace netLib::net;

void netLib::net::defaultConnectionCallback(const TcpConnectionPtr& conn)
{
    LOG_TRACE << conn->localAddress().toIpPort() << " -> "
                << conn->peerAddress().toIpPort() << " is "
                << (conn->connected() ? "UP":"DOWN");
    // do not call conn->forceClose(), because some user want to register message callback()
}   

void netLib::net::defaultMessageCallback(const TcpConnectionPtr&,
                                        Buffer* buf,
                                        Timestamp)
{
    buf->retrieveAll();
}

TcpConnection::TcpConnection(EventLoop* loop,
                            const string& nameArg,
                            int sockfd,
                            const InetAddress& localAddr,
                            const InetAddress& peerAddr)
    :loop_(CHECKNOTNULL(loop)),
    name_(nameArg),
    state_(kConnecting),
    reading_(true),
    socket_(new Socket(sockfd)),        // 创建套接字
    channel_(new Channel(loop, sockfd)),
    localAddr_(localAddr),
    peerAddr_(peerAddr),
    highWaterMark_(64*1024*1024)
{
    channel_->setReadCallback(
        std::bind(&TcpConnection::handleRead, this, _1)
    );
    channel_->setWriteCallback(
        std::bind(&TcpConnection::handleWrite, this)
    );
    channel_->setCloseCallback(
        std::bind(&TcpConnection::handleClose, this)
    );
    channel_->setErrorCallback(
        std::bind(&TcpConnection::handleError, this)
    );
    LOG_DEBUG << "TcpConnection::ctor[" << name_ <<  "] at" << this
                << " fd=" << sockfd;
    socket_->setKeepAlive(true);
}

TcpConnection::~TcpConnection()
{
    LOG_DEBUG << "TcpConnection::dtor[" << name_ << "] at " << this
                << " fd= " << channel_->fd()
                << " state= " << stateToString();
    assert(state_ == kDisconnected); 
}

bool TcpConnection::getTcpinfo(struct tcp_info* tcpi) const
{
    return socket_->getTcpInfo(tcpi);
}

string TcpConnection::getTcpinfoString() const
{
    char buf[1024];
    buf[0] = '\0';
    socket_->getTcpInfoString(buf, sizeof(buf));
    return buf;
}

void TcpConnection::send(const void* data, int len)
{
    send(StringPiece(static_cast<const char*>(data), len));
}

void TcpConnection::send(const StringPiece& message)
{
    if(state_ == kConnected)
    {   // if the in LoopThread, sendInLoop()
        // else, call the runInLoop of the loop thread 
        if(loop_->isInLoopThread())
        {
            sendInLoop(message);
        }
        else
        {
            void (TcpConnection::*fp)(const StringPiece& message) = &TcpConnection::sendInLoop;
            loop_->runInLoop(
                std::bind(fp,
                        this,
                        message.as_string()));
        }
    }
}

void TcpConnection::send(Buffer* buf)
{
    if(state_ == kConnected)
    {
        if(loop_->isInLoopThread())
        {
            sendInLoop(buf->peek(), buf->readableBytes());
            buf->retrieveAll();
        }
        else
        {
            void (TcpConnection::*fp)(const StringPiece&  message) = &TcpConnection::sendInLoop;
            loop_->runInLoop(
                std::bind(fp,
                        this, 
                        buf->retrieveAllAsString()));

        }
    }
}

void TcpConnection::sendInLoop(const StringPiece& message)
{
    sendInLoop(message.data(), message.size());
}

void TcpConnection::sendInLoop(const void* data, size_t len)
{
    loop_->assertInLoopThread();
    ssize_t nwrote = 0;
    size_t remaining = len;
    bool faultError = false;
    if(state_ == kDisconnected)
    {
        LOG_WARN << "disconnected, give up writing ";
        return;
    }
    // if no thing is in output queue, try writing directly 
    if(!channel_->isWriting() && outputBuffer_.readableBytes() == 0)
    {
        nwrote = sockets::write(channel_->fd(), data, len);
        if(nwrote >= 0)
        {
            remaining = len - nwrote;
            if(remaining == 0 && writeCompleteCallback_)
            {
                loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
            }
        }
        else// nworte <= 0 
        {
            nwrote = 0;
            if(errno != EWOULDBLOCK)
            {
                LOG_SYSERROR << "TcpConnection::sendInLoop";
                if(errno == EPIPE || errno == ECONNRESET)
                {
                    faultError = true;
                }
            }
        }
    }

    assert(remaining <= len);
    if(!faultError && remaining > 0)
    {
        size_t oldLen = outputBuffer_.readableBytes();
        if(oldLen+remaining >= highWaterMark_
            && oldLen < highWaterMark_
            && highWaterMarkCallback_)
        {
            loop_->queueInLoop(std::bind(highWaterMarkCallback_, shared_from_this(), oldLen+remaining));
        }
        outputBuffer_.append(static_cast<const char*> (data)+nwrote, remaining);
        if(!channel_->isWriting())
        {
            channel_->enableWriting();
        }
    }
}

void TcpConnection::shutdown()
{
    if(state_ == kConnected)
    {
        setState(kDisConnecting);
        loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));   
    }
}

void TcpConnection::shutdownInLoop()
{
    loop_->assertInLoopThread();
    if(!channel_->isWriting())
    {
        socket_->shutdownWrite();
    }
}

void TcpConnection::forceClose()
{
    if(state_ == kConnected || state_ == kDisConnecting)
    {
        setState(kDisConnecting);
        loop_->queueInLoop(std::bind(&TcpConnection::forceCloseInLoop, shared_from_this()));
    }
}

void TcpConnection::forceCloseWithDelay(double seconds)
{
    if(state_ == kConnected || state_ == kDisconnected)
    {
        setState(kDisConnecting);
        loop_->runAfter(
            seconds,
            makeWeakCallback(shared_from_this(),
                            &TcpConnection::forceClose));
    }
}

void TcpConnection::forceCloseInLoop()
{
    loop_->assertInLoopThread();
    if(state_ == kConnected || state_ == kDisConnecting)
    {
        handleClose();
    }
}

const char* TcpConnection::stateToString() const
{
    switch(state_)
    {
        case kDisconnected:
            return "kDisconnected";
        case kConnecting:
            return "kConnecting";
        case kConnected:
            return "kConnected";
        case kDisConnecting:
            return "kDisConnecting";
        default:
            return "unkown state";
    }
}

void TcpConnection::setTcpNoDelay(bool on)
{
    socket_->setTcpNoDelay(on);
}

void TcpConnection::startRead()
{
    loop_->runInLoop(std::bind(&TcpConnection::startReadInLoop, this));
}

void TcpConnection::startReadInLoop()
{
    loop_->assertInLoopThread();
    if(!reading_ || !channel_->isReading())
    {
        channel_->enableReading();
        reading_ = true;
    }
}

void TcpConnection::stopRead()
{
    loop_->runInLoop(std::bind(&TcpConnection::stopReadInLoop, this));
}
void TcpConnection::stopReadInLoop()
{
    loop_->assertInLoopThread();
    if(reading_ || channel_->isReading())
    {
        channel_->disableReading();
        reading_ = false;
    }
}       

void TcpConnection::connectEstablished()
{
    loop_->assertInLoopThread();
    assert(state_ == kConnecting);
    setState(kConnected);
    channel_->tie(shared_from_this());
    channel_->enableReading();

    connectionCallback_(shared_from_this());
}

void TcpConnection::connectDestoryed()
{
    loop_->assertInLoopThread();
    if(state_ == kConnected)
    {
        setState(kDisconnected);
        channel_->disableAll();

        connectionCallback_(shared_from_this());
    }
    channel_->remove();
}

void TcpConnection::handleRead(Timestamp receiveTime)
{
    loop_->assertInLoopThread();
    int savedErrno = 0;
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
    if(n > 0)
    {
        messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
    }
    else if(n == 0)
    {
        handleClose();
    }
    else
    {
        errno = savedErrno;
        LOG_SYSERROR << "TcpConenction::handleRead";
        handleError();
    }
}

void TcpConnection::handleWrite()
{
    loop_->assertInLoopThread();
    if(channel_->isWriting())
    {
        ssize_t n = sockets::write(channel_->fd(),
                                    outputBuffer_.peek(),
                                    outputBuffer_.readableBytes());
        if(n > 0)
        {
            outputBuffer_.retrieve(n);
            if(outputBuffer_.readableBytes() == 0)
            {
                channel_->disableWriting();
                if(writeCompleteCallback_)
                {
                    loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
                }
                if(state_ == kDisConnecting)
                {
                    shutdownInLoop();
                }
            }
        }
        else
        {
            LOG_SYSERROR  << "TcpConnection::handleWrite";
        }
    }
    else
    {
        LOG_TRACE << "Connection fd = " << channel_->fd() 
                    << " is down, no more writing ";
    }
}

void TcpConnection::handleClose()
{
    loop_->assertInLoopThread();
    LOG_TRACE << "fd = " << channel_->fd() << " state = " << stateToString();
    assert(state_ == kConnected || state_ == kDisConnecting);
    // we don't close fd, leave it to dtor, so we can find leaks easily
    setState(kDisconnected);
    channel_->disableAll();

    TcpConnectionPtr guardThis(shared_from_this());
    connectionCallback_(guardThis);
    // must be the last 
    closeCallback_(guardThis);
}

void TcpConnection::handleError()
{
    int err = sockets::getSocketError(channel_->fd());
    LOG_ERROR << "TcpConnection::handleError [" << name_ 
                << "] - SO_ERROR = " << err << " " <<strerror_tl(err);
}