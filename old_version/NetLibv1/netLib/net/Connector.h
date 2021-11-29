#pragma once

#include "../base/noncopyable.h"
#include "../net/InetAddress.h"

#include <functional>
#include <memory>

namespace netLib{
namespace net
{

class Channel;
class EventLoop;

// Conenctor also init the loop_ and serverAddr_, state_, retryDelays_
//1.  like the TcpConnection startReadInLoop, ,it also have the startInloop and stopInLoop
//2 .the startInLoop will call "connect()", and call sockfd = sockets::createNonBlockingOrDie(serverAddr.family) and use the sockfd to connect with serverAddr 
//3. stop also place the stopInLoop to queueInLoop
//4. the retry function also define  
class Connector:noncopyable,
                public std::enable_shared_from_this<Connector>
{
public:
    typedef  std::function<void (int sockfd)> NewConnectionCallback;

    Connector(EventLoop* loop, const InetAddress& serveAddr);

    ~Connector();

    void setNewConnectionCallback(const NewConnectionCallback& cb)
    { newConnectionCallback_ = cb; }
    

    void start();           //can be called in any thread

    void restart();         //must be called in loop thread;

    void stop();            //can be called in any thread

    const InetAddress& serverAddress() const { return serverAddr_; }
private:
    enum States {kDisconnected, kConnecting, kConnected };
    static const int kMaxRetryDelayMs = 30*1000;
    static const int kInitRetryDelayMs = 500;

    void setState(States s) { state_ = s; }

    void startInLoop();

    void stopInLoop();

    void connect();
    // connecting set the Writecallback and Errorcallback of channel, and enabe reading 
    void connecting(int sockfd);

    void handleWrite();

    void handleError();
    // close sockfd, and use startInloop() the Connector, and update the retrytimes
    void retry(int sockfd);
    // removeAndResetChannel, diableAll and remove Channel in the loop
    int removeAndResetChannel();

    void resetChannel();

    EventLoop* loop_;
    InetAddress serverAddr_;
    bool connect_;      //atomic
    States state_;
    std::unique_ptr<Channel> channel_;
    NewConnectionCallback newConnectionCallback_;
    int retryDelayMs_;
};

} // namespace net
    
}