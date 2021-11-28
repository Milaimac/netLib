#include "../net/TcpClient.h"

#include "../base/Logging.h"
#include "../net/Connector.h"
#include "../net/EventLoop.h"
#include "../net/SocketsOps.h"

#include <stdio.h>
using namespace netLib;
using namespace netLib::net;

namespace netLib
{
namespace  net
{
namespace detail
{
void removeConnection(EventLoop* loop, const TcpConnectionPtr& conn)
{
    loop->queueInLoop(std::bind(&TcpConnection::connectDestoryed, conn));
}

void removeConnector(const ConnectorPtr& connector)
{
    // connector->
}
}// namespace detail
}// namespace net   
}// namespace netLib


using namespace net;
using namespace netLib::net;

TcpClient::TcpClient(EventLoop* loop, 
                    const InetAddress& serverAdddr,
                    const string& nameArg)
:loop_(CHECKNOTNULL(loop)),
connector_(new Connector(loop, serverAdddr)),
name_(nameArg),
connectionCallback_(defaultConnectionCallback),
messageCallback_(defaultMessageCallback),
retry_(false),
connect_(true),
nextConnId_(1)
{
    // 将connection 设置为TcpClient的
    connector_->setNewConnectionCallback(
        std::bind(&TcpClient::newConnection, this, _1));
    LOG_INFO << "TcpClient::TcpClient[" << name_ 
                << "] - connector " << get_pointer(connector_); 
}

TcpClient::~TcpClient()
{
    LOG_INFO << "TcpClient::~TcpClient [" << name_  
                << "] - connector " << get_pointer(connector_);
    TcpConnectionPtr conn;
    bool unique  = false;

    {// lock and assure the atomic of unique and conn 
    MutexLockGuard lock(mutex_);
    unique = connection_.unique();  //unique 是 connection_ 的 shareptr是否只有一个对象使用它
    conn = connection_;
    }

    if(conn)
    {
        assert(loop_ == conn->getLoop());
        CloseCallback cb = std::bind(&detail::removeConnection, loop_, _1);
        loop_->runInLoop(
            std::bind(&TcpConnection::setCloseCallback, conn, cb));
        if(unique)  //如果只有一个连接， 直接关掉
        {
            conn->forceClose();
        }
    }
    else// 如果没有连接， 就让connector停止连接
    {
        connector_->stop();
        loop_->runAfter(1, std::bind(&detail::removeConnector, connector_));
    }
}

void TcpClient::connect()
{
    LOG_INFO << "TcpClient::connect[" << name_ << "] - connecting to " 
                << connector_->serverAddress().toIpPort();
    connect_ = true;
    connector_->start();
}

void TcpClient::disconnect()
{
    connect_ = false;
    {
    MutexLockGuard lock(mutex_);
    if(connection_)
    {
        connection_->shutdown();
    }
    }
}

void TcpClient::stop()
{
    connect_ = false;
    connector_->stop();
}

// 先从sockfd 得到对端的ip地址和端口号
// 然后获得本地地 localAddr
// 用这个创建一个TcpCoonection， 设置好对应的回调函数
// 将 conn（新建的对象） 赋予 TcpClient的connection_

void TcpClient::newConnection(int sockfd)
{
    loop_->assertInLoopThread();
    InetAddress peerAddr(sockets::getPeerAddr(sockfd));
    char buf[32];
    snprintf(buf, sizeof buf, ":%s#%d", peerAddr.toIpPort().c_str(), nextConnId_);
    ++nextConnId_;
    string connName = name_ + buf;

    InetAddress localAddr(sockets::getLocalAddr(sockfd));

    TcpConnectionPtr conn(new TcpConnection(loop_, 
                            connName, 
                            sockfd,
                            localAddr, 
                            peerAddr));
    
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(
        std::bind(&TcpClient::removeConnection, this, _1));
    
    {
    MutexLockGuard lock(mutex_);
    connection_ = conn;
    }

    conn->connectEstablished();
}


void TcpClient::removeConnection(const TcpConnectionPtr& conn)
{
    loop_->assertInLoopThread();
    assert(loop_ == conn->getLoop());

    {
    MutexLockGuard lock(mutex_);
    assert(connection_ == conn);
    connection_.reset();
    }

    loop_->queueInLoop(std::bind(&TcpConnection::connectDestoryed, conn));
    if(retry_ && connect_)
    {
        LOG_INFO << "TcpClient::connect[" << name_ << "] - Reconnecting to "
                    << connector_->serverAddress().toIpPort();
        connector_->restart();
    }
}