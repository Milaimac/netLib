#include "../net/EventLoop.h"

#include "../base/Logging.h"
#include "../base/Mutex.h"

#include "../net/Poller.h"
#include "../net/Channel.h"
#include "../net/SocketsOps.h"
#include "../net//TimerQueue.h"

#include <algorithm>
#include <sys/eventfd.h>
#include <unistd.h>

using namespace netLib;
using namespace netLib::net;

// uname namespace
namespace {

__thread EventLoop* t_loopInThisthread = 0;
const int kPollTimeMs = 10000;

// eventfd 是一种计数器
int createEventfd(){
    int evtfd = ::eventfd(0, EFD_NONBLOCK|EFD_CLOEXEC);  
    if(evtfd < 0){
        LOG_SYSERROR << "create eventfd error";
        abort();
    }
    return evtfd;
}



} //namespace


EventLoop* EventLoop::getEventLoopOfCurrentThread(){
    return t_loopInThisthread;
}

EventLoop::EventLoop()
    :looping_(false),
    quit_(false),
    eventHandling_(false),
    callingPendingFunctors_(false),
    iteration_(0),
    threadId_(CurrentThread::tid()),
    poller_(Poller::newDefaultPoller(this)),
    timerQueue_(new TimerQueue(this)),
    wakeupFd_(createEventfd()),
    wakeupChannel_(new Channel(this, wakeupFd_)),
    currentActiveChannel_(nullptr)
{
    if(t_loopInThisthread)
    {
        LOG_FATAL << "Another EventLoop " << t_loopInThisthread 
                << " exits this thread " << threadId_;  
    }
    else{
        t_loopInThisthread = this;
    }
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableReading();
}

EventLoop::~EventLoop(){
    LOG_TRACE << "EventLoop "  << this << " of thread " << threadId_ 
                << " destruct in thread " << CurrentThread::tid();
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    ::close(wakeupFd_);
    t_loopInThisthread = nullptr;
}
// the loop() will use Poller_->poll(kPollTiemMs, &activeChannels_)
// the from activeChannels_ ,do the element of the Channels_;
// and then the pending Functors will be done when the channels_ have been done 
void EventLoop::loop(){
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    quit_ = false;
    LOG_TRACE << "EventLoop " << this 
                << " start Looping ";

    while(!quit_){
        activeChannels_.clear();
        pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
        
        if(Logger::logLevel() <= Logger::TRACE){
            printActiveChannels();
        }
        eventHandling_ = true;
        // 交给Channel 分发事件
        for(Channel* channel: activeChannels_){
            currentActiveChannel_ = channel;
            currentActiveChannel_->handleEvent(pollReturnTime_);            // here need the channel to handleEvent
        }   
        currentActiveChannel_ = nullptr;
        eventHandling_ = false;
        doPendingFunctors();
    }

    LOG_TRACE << "EventLoop " << this << " stop Looping";
    looping_ = false;
}

void EventLoop::quit(){
    quit_ = true;
    // there is a chance to set the quit_ to false, and exeute the loop(), 
    // it will run "while(!quit_)"
    // and then pass it, then stop looping, then then EventLoop will destructs
    if(!isInLoopThread()){  // 如果当前线程不是该loop的线程，那么唤醒它
        wakeup();
    }
}

void EventLoop::runInLoop(Functor cb){
    if(isInLoopThread())
    {
        cb();
    }
    else
    {
        queueInLoop(std::move(cb));
    }
}

void EventLoop::queueInLoop(Functor cb){
    {
    MutexLockGuard lock(mutex_);
    pendingFunctors_.push_back(std::move(cb));
    }

    if(!isInLoopThread() || callingPendingFunctors_){
        wakeup();
    }
}

size_t EventLoop::queueSize() const{
    MutexLockGuard lock(mutex_);
    return pendingFunctors_.size();
}

TimerId EventLoop::runAt(Timestamp time, TimerCallback cb){
    return timerQueue_->addTimer(std::move(cb), time, 0.0); //no interval
}

TimerId EventLoop::runAfter(double delay, TimerCallback cb){
    Timestamp time(addTime(Timestamp::now(), delay));
    return runAt(time, std::move(cb));    
}

TimerId EventLoop::runEvery(double interval, TimerCallback cb){
    Timestamp time(addTime(Timestamp::now(), interval));
    return timerQueue_->addTimer(std::move(cb), time, interval);
}

void EventLoop::cancel(TimerId timerId){
    timerQueue_->cancel(timerId);
}

void EventLoop::updateChannel(Channel* channel){
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel){
    assert(channel->ownerLoop() == this);
    assertInLoopThread();

    if(eventHandling_){// if handling event
        //asure the currentAcitveChannel_ in the eventLoop  and find the activeChannels 
        assert(currentActiveChannel_ == channel ||
                std::find(activeChannels_.begin(), activeChannels_.end(), channel) == activeChannels_.end());
    }
    poller_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel* channel){
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    return poller_->hasChannel(channel);
}

void EventLoop::abortNotInLoopThread(){
    LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this 
                <<  " was created in threadId_ = " << threadId_ 
                << ", current thread id = " << CurrentThread::tid(); 
}

// wakeup the thread by write 1 byte
void EventLoop::wakeup(){
    uint64_t one = 1;
    ssize_t n = ::write(wakeupFd_, &one, sizeof(one));
    if(n != sizeof(one)){
        LOG_SYSERROR << "EventLoop::wakeup() writes " << n << " bytes instend of 8";
    }
}
void EventLoop::handleRead(){
    uint64_t one = 1;
    ssize_t n =::read(wakeupFd_, &one, sizeof(one));
    if(n != sizeof(one)){
        LOG_SYSERROR << "EventLoop::handleRead() reads "<< n << " bytes insread of 8";
    }
}

void EventLoop::doPendingFunctors(){
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;
    {
    MutexLockGuard lock(mutex_);
    functors.swap(pendingFunctors_);
    }
    for(const Functor& functor:functors){
        functor();
    }
    callingPendingFunctors_ = false;
}

void EventLoop::printActiveChannels() const{
    for(const Channel* channel:activeChannels_){
        LOG_TRACE << "{ "  << channel->reventsToString() << " }";
    }
}