#pragma once

#include "../base/Mutex.h"
#include "../net/TcpConnection.h"

namespace netLib
{
namespace net
{

class Connector;
typedef std::shared_ptr<Connector> ConnectorPtr;

// 客户端需要实现
// conenct, disconnect, stop, set some callback, 
class TcpClient
{
public:
    // TcpClient(EventLoop *loop, const string& host, uin16_t port)
    TcpClient(EventLoop *loop, 
                const InetAddress& serveAddr, 
                const string& nameArg);
        
    ~TcpClient();

    void connect();

    void  disconnect();

    void stop();

    TcpConnectionPtr connection() const
    {
        MutexLockGuard lock(mutex_);
        return connection_;
    }

    EventLoop* getloop(){
        return loop_;
    }

    bool retry() const 
    {
        return retry_;
    }
    
    void enableRetry()
    {
        retry_ = true;
    }

    const string name()
    {
        return name_;
    }
    
    // set callback
    // use the right reference , and increase the efficency 
    void setConnectionCallback(ConnectionCallback cb) 
    {
        connectionCallback_ = std::move(cb);
    }

    void setMessageCallback(MessageCallback cb)
    {
        messageCallback_ = std::move(cb);
    }

    void setWriteCompleteCallback(WriteCompleteCallback cb)
    {
        writeCompleteCallback_ = std::move(cb);
    }



            
private:
    // not thread safe, but in loop
    void newConnection(int sockfd);

    //not thread safe, but in loop
    void removeConnection(const TcpConnectionPtr& conn);

    EventLoop* loop_;
    ConnectorPtr connector_;         // avoid reveal connector
    const string name_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    bool retry_;
    bool connect_;
    int nextConnId_;
    mutable Mutex mutex_;
    TcpConnectionPtr connection_;        //guard by (mtuex_)
};

    
} // namespace net

}