#include "../net/TcpServer.h"

#include "../base/Logging.h"
#include "../net/Acceptor.h"
#include "../net/EventLoop.h"
#include "../net/EventLoopThreadPool.h"
#include "../net/SocketsOps.h"

using namespace netLib;
using namespace netLib::net;

// the TcpSever just need to
// init the ipPort and set the defaultConnectionCallback and defaultMessageCallback ,thread pool,  Accpetor

// function
// 1. newConnection by the peerAddr and sockfd,
// the ioloop from the threadpool will exeute the connectionEstablised callback, and the thread and ioLoop is got from the threadPool
// and stroe the TcpConnection "conn" to connections_  and do the connecEstablished
// 2. remove connection, (1) erase the element in the connections_, (02) ioLoop->queueInLoop(connectDestoryed), it will disable the channel
TcpServer::TcpServer(EventLoop *loop,
                     const InetAddress &listenAddr,
                     const string &nameArg,
                     Option option)
    : loop_(CHECKNOTNULL(loop)),
      ipPort_(listenAddr.toIpPort()),
      name_(nameArg),
      acceptor_(new Acceptor(loop_, listenAddr, option == kReusePort)),
      threadPool_(new EventLoopThreadPool(loop_, nameArg)),
      connectionCallback_(defaultConnectionCallback),
      messageCallback_(defaultMessageCallback),
      nextConnId_(1)
{
    acceptor_->setNewConnectionCallback(
        std::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{
    loop_->assertInLoopThread();
    LOG_TRACE << "TcpServer::~Tcpserver [" << name_ << "] destructing";
    for (auto &item : connections_)
    {
        TcpConnectionPtr conn(item.second);
        item.second.reset();
        conn->getLoop()->runInLoop(
            std::bind(&Acceptor::listen, get_pointer(acceptor_)));
    }
}
// with peeraddr, they connect
// the buf is the the ipPort and nextConnId_
// 1. from sockfd get the localAddr
// 2. and new TcpConnection object then set the callback to it

void TcpServer::newConnection(int sockfd, const InetAddress &peerAddr)
{
    loop_->assertInLoopThread();
    EventLoop *ioLoop = threadPool_->getNextLoop();
    char buf[64];
    snprintf(buf, sizeof buf, "-%s#%d", ipPort_.c_str(), nextConnId_);
    ++nextConnId_;
    string connName = name_ + buf;

    LOG_INFO << "TcpServer::newConnection [" << name_
             << "] - new conection [" << connName
             << "] from " << peerAddr.toIpPort();
    InetAddress localAddr(sockets::getLocalAddr(sockfd));
    // FIXME poll with the zero timeout to doubel confirm the connection
    //FIXME if neccessary, use shared_ptr
    TcpConnectionPtr conn(new TcpConnection(
        ioLoop,
        connName,
        sockfd,
        localAddr,
        peerAddr));
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(std::bind(
        &TcpServer::removeConnection, this, _1));
    ioLoop->runInLoop(
        std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr &conn)
{
    loop_->runInLoop(
        std::bind(&TcpServer::removeConnecitonInLoop, this, conn));
}

void TcpServer::removeConnecitonInLoop(const TcpConnectionPtr &conn)
{
    loop_->assertInLoopThread();
    LOG_INFO << "TcpServer::removeConnectionInLoop [ " << name_ << "] - connection " << conn->name();
    size_t n = connections_.erase(conn->name());
    (void)n;
    assert(n == 1);
    EventLoop *ioloop = conn->getLoop();
    ioloop->queueInLoop(std::bind(&TcpConnection::connectDestoryed, conn));
}