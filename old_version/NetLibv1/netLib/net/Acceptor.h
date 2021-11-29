/*
 * @Date: 2021-11-05 11:09:34
 * @LastEditors: kafier
 * @LastEditTime: 2021-11-08 09:13:24
 * @FilePath: /netLib/net/Acceptor.h
 */
#pragma once
#include <functional>

#include "../net/Socket.h"
#include "../net/Channel.h"

// Acceptor class should use listen, and build connector socket 

namespace netLib{
namespace net{

// front declare
class EventLoop;
class InetAddress;


class Acceptor : noncopyable{
public:
    typedef std::function<void (int sockfd, const InetAddress&)> NewconncetionCallback;

    Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
    ~Acceptor();

    void setNewConnectionCallback(const NewconncetionCallback& cb){
        newconncetionCallback_ = cb;
    }

    void listen();

    bool listening(){
        return listening_;
    }
private:
    void handleRead();

    EventLoop* loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewconncetionCallback newconncetionCallback_;
    bool listening_;
    int idleFd_;
};

}    
}
