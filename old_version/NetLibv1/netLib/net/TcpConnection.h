#pragma once

#include "../base/noncopyable.h"
#include "../base/StringPiece.h"
#include "../base/Types.h"
#include "../net/Callbacks.h"
#include "../net/Buffer.h"
#include "../net/InetAddress.h"

#include <memory>

#include <boost/any.hpp>


//struct tpc_info is in <netinet/tcp.h>
struct tcp_info;

namespace netLib
{
namespace net
{

class Channel;
class EventLoop;
class Socket;

//
// TCP conection, for both client and server
// 
// This is interface class ,don't expose too many details


// Tcp Conection 
// the handleRead, handleWrite, handleError, handleClose these function will allocate to the channel
// the send function is use the runInLoop in the EventLoop and sendInLoop in the TcpConnection;
// startReadInLoop and stopReedInLoop use the channel    
// forceclose set the state to kDisconnected and it will be run in runInLoop in the EventLoop

// 可以知道TcpConnection不是用来connect的， 而是用来管理connection的
class TcpConnection : noncopyable, public std::enable_shared_from_this<TcpConnection>
{
public:
    // constructs a Tcpconnection with a connected sockfd
    // User should not create this object

    TcpConnection(EventLoop* loop,
                    const string& name,
                    int sockfd,
                    const InetAddress& localAddr,
                    const InetAddress& peerAddr);

    ~TcpConnection();
    EventLoop* getLoop() const { return loop_; }
    const string& name() const { return name_; }
    const InetAddress& localAddress() const { return localAddr_; }
    const InetAddress& peerAddress() const { return peerAddr_; }
    bool connected() const { return state_ == kConnected; }
    bool disconnected() const { return state_ == kDisconnected; }
    // return true if success 
    bool getTcpinfo(struct tcp_info*) const;
    string getTcpinfoString() const;
    void send(string&& message);
    void send(const void* message, int len);
    void send(const StringPiece& message);
    void send(Buffer* message);     //this one will swap data
    void shutdown();
    void forceClose();
    void forceCloseWithDelay(double seconds);
    void setTcpNoDelay(bool on);
    void startRead();
    void stopRead();
    bool isReading() const { return reading_; }     // Not thread safe, may race with start/stopReadInLoop

    void setContext(const boost::any& context)
    { context_ = context; }
    
    const boost::any& getContext() const
    { return context_; }

    boost::any* getMutableContext()
    { return &context_; }

    void setConnectionCallback(const ConnectionCallback& cb)
    {
        connectionCallback_ = cb;
    }

    void setMessageCallback(const MessageCallback& cb)
    {
        messageCallback_ = cb;
    }

    void setWriteCompleteCallback(const WriteCompleteCallback& cb)
    {
        writeCompleteCallback_ = cb;
    }

    void setHighWaterMarkCallback(const HighWaterMarkCallback& cb, size_t highWaterMark)
    {
        highWaterMarkCallback_ = cb;
        highWaterMark_ = highWaterMark;
    }

    Buffer* inputBuffer()
    {
        return &inputBuffer_;
    }

    Buffer* outputBuffer()
    {
        return &outputBuffer_;
    }


    // Internal use only
    void setCloseCallback(const CloseCallback& cb)
    {
        closeCallback_ = cb;
    }

    // called when Tcpserver accepts a new connection
    void connectEstablished();          //should be called only once
    // called when Tcpserver has removed me from its map
    void connectDestoryed();            //should be called only once

private:
    enum StateE {kDisconnected, kConnected, kConnecting, kDisConnecting };
    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();
    // void sendInLoop(string&& message);
    void sendInLoop(const StringPiece& message);
    void sendInLoop(const void* message, size_t len);
    void shutdownInLoop();
    void forceCloseInLoop();

    void setState(StateE s) { state_ = s; }
    const char* stateToString() const;
    void startReadInLoop();
    void stopReadInLoop();

    EventLoop* loop_;
    const string name_;
    StateE state_;
    bool reading_;

    // we don't expose those classes to client
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    const InetAddress localAddr_;
    const InetAddress peerAddr_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    HighWaterMarkCallback highWaterMarkCallback_;
    CloseCallback closeCallback_;
    size_t highWaterMark_;
    Buffer inputBuffer_;
    Buffer outputBuffer_;   //FIXME   use list<Buffer> as output buffer. 
    boost::any context_;
};
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
} // namespace net

} // namespace netLib