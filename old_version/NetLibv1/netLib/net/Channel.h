#pragma once
#include "../base/noncopyable.h"
#include "../base/Timestamp.h"

#include <string>
#include <memory>
#include <functional>
namespace  netLib{
namespace net{

class EventLoop;

class Channel:noncopyable{
public:
    typedef std::function<void()> EventCallback;
    typedef std::function<void(Timestamp)> ReadEventCallback;

    Channel(EventLoop* loop, int fd);       //传入Loop对象， one loop per thread, 传入fd， 负责fd中事件的分发，而不负责管理fd
    ~Channel();

    void handleEvent(Timestamp receiveTime);

    void setReadCallback(ReadEventCallback cb){
        readCallback_ = std::move(cb);
    }
    void setWriteCallback(EventCallback cb){
        writeCallback_ = std::move(cb);
    }
    void setCloseCallback(EventCallback cb){
        closeCallback_ = std::move(cb);
    }
    void setErrorCallback(EventCallback cb){
        errorCallback_ = std::move(cb);
    }

    // tie this Channel to the owner object by shared ptr 
    // to prevent the owner object to be destoryed in handleEvent
    void tie(const std::shared_ptr<void>&);

    int fd() {return fd_;}
    int events(){return events_;}
    
    void set_revents(int evt){ revents_ = evt; }
    
    void enableReading(){ events_ |= kReadEvent; update(); }
    void disableReading(){ events_ &= ~kReadEvent; update();}
    void enableWriting() {events_ |= kWriteEvent; update();}
    void disableWriting() { events_ &= ~kWriteEvent; update();}
    void disableAll() {events_ = kNoneEvent; update(); }
    bool isReading() const { return events_ & kReadEvent; }
    bool isWriting() { return events_ & kWriteEvent; }
    bool isNoneEvent() { return events_ ==  kNoneEvent; }

    // for Poller
    int index() {   return index_; }
    void set_index(int idx){ index_ = idx; }
    // for debug
    std::string reventsToString() const;
    std::string eventsToString() const;
    void doNotLogHup() { logHup_ = false; }
    EventLoop* ownerLoop() {return loop_; }
    void remove();

private:
    static std::string eventsToString(int fd, int ev);
    void update();
    void handelEventwithGuard(Timestamp receiveTime);

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop* loop_;
    const int fd_;
    int events_;
    int revents_;   //it's the received event types for epoll or poll
    int index_;     //used by poller        it should be initalized to -1
    
    bool logHup_;   // how it work?     

    std::weak_ptr<void> tie_;       //use to tie to the object          
    bool tied_;
    bool eventHandling_;
    bool addedToLoop_;
    // ReadEventCallback 是由 std::function<void(TimeStamp)> ReadEventCallback;
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;
};


}   //net
}   //netLib