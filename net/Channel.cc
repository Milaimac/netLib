#include "../base/Logging.h"
#include "Channel.h"
#include "EventLoop.h"
#include <poll.h>
#include <assert.h>
using namespace netLib;
using namespace netLib::net;



const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd)
    :fd_(fd),
    loop_(loop), 
    events_(0),
    revents_(0),
    index_(-1), 
    logHup_(true),  // 用来处理POLLHUP事件 （文件描述符挂起
    tied_(false),
    eventHandling_(false),
    addedToLoop_(false)
{

}

Channel::~Channel(){
    // must assert no in handling event and not added To Loop
    assert(!eventHandling_);    
    assert(!addedToLoop_);
    if(loop_->isInLoopThread())
    {
        assert(!loop_->hasChannel(this));
    }
}

void Channel::tie(const std::shared_ptr<void>& obj){
    tie_ = obj;
    tied_ = true;
}

void Channel::update(){
    addedToLoop_ = true;
    loop_->updateChannel(this);
}

void Channel::remove(){
    assert(isNoneEvent());
    addedToLoop_ = false;
    loop_->removeChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime){
    std::shared_ptr<void> guard;
    if(tied_){
        guard = tie_.lock();
        if(guard){
            handelEventwithGuard(receiveTime);
        }
    }
    else{
        handelEventwithGuard(receiveTime);
    }
}

void Channel::handelEventwithGuard(Timestamp receiveTime){
    eventHandling_ = true;
    LOG_TRACE << reventsToString();
    if((revents_ & POLLHUP) && !(revents_ & POLLIN)){
        // 如果对方文件描述符挂起， 并且没有可读数据
        if(logHup_){
            LOG_WARN << "fd = " << fd_ << "Channel::hand_event() POLLHUP";
        }
        if(closeCallback_)  closeCallback_();
    }
    if(revents_ & POLLNVAL){
        LOG_WARN << "fd = " << fd_ << "Channel::hand_event() POLLNVAL";
    }
    if(revents_ & (POLLNVAL | POLLERR)){
        if(errorCallback_)  errorCallback_();
    }
    if(revents_ & (POLLIN | POLLPRI | POLLHUP)){
        if(readCallback_)   readCallback_(receiveTime);
    }
    if(revents_ & (POLLOUT)){
        if(writeCallback_)  writeCallback_();
    }
    eventHandling_ = false;
}

string Channel::reventsToString() const {
    return eventsToString(fd_, events_);
}

string Channel::eventsToString() const {
    return eventsToString(fd_, events_);
}


// POLLIN 普通或优先级数据可读  POLLPRI 优先级数据可读
// POLLOUT 普通数据可写
// POLLHUP 对方描述符挂起   
string Channel::eventsToString(int fd, int ev){
    std::ostringstream oss;
    oss << fd << ": ";
    if(ev & POLLIN)
        oss << "IN ";
    if(ev & POLLPRI)
        oss << "PRI ";
    if(ev & POLLOUT)
        oss << "OUT ";
    if(ev & POLLHUP)
        oss << "HUP ";
    if(ev & POLLRDHUP)
        oss << "RDHUP ";
    if(ev & POLLERR)
        oss << "ERR";
    if(ev & POLLNVAL)
        oss << "NVAL";
    return oss.str();
}
