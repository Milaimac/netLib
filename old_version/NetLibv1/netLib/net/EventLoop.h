#pragma once
#include <atomic>
#include <functional>
#include <vector>

#include <boost/any.hpp>

#include "../base/Mutex.h"
#include "../base/CurrentThread.h"
#include "../base/Timestamp.h"
#include "../net/Callbacks.h"
#include "../net/TimerId.h"

namespace netLib{
namespace net{
class Channel;
class Poller;
class TimerQueue;

class EventLoop:noncopyable{
public:
    typedef  std::function<void()> Functor;
    EventLoop();
    ~EventLoop();

    // 
    // Loops forever 
    //
    // must be called in the same thread as creation of the  object

    void loop();

    // quits loop
    //
    // this is not 100% thread safe, if you call through a raw pointer
    // better to call through shared_ptr<EventLoop> for 100 % thread_safe

    void quit();

    // 
    // Time when poll returns, usually means data arrival

    Timestamp pollReturnTime() const {return pollReturnTime_; }

    int64_t iteration() const {return iteration_; }

    // runs callback immediately in the loop thread
    // It wake up the loop, and run the cb 
    // If in the same loop thread, cb is run within the function
    // Safe to call from other threads

    void runInLoop(Functor cb);
    // Queues callback in the loop thread
    // Runs after finish pooling
    // Safe to call from othre threads;

    void queueInLoop(Functor cb);

    size_t queueSize() const;

    // timers

    // run callback at 'time'
    // Safe to from other threads
    //
    TimerId runAt(Timestamp time, TimerCallback cb);

    // run callback after delay seconds
    TimerId runAfter(double delay, TimerCallback cb);

    // run callback every interval seconds
    TimerId runEvery(double interval, TimerCallback cb);

    // Cancel this timer
    void cancel(TimerId TimerId);

    // interval usage
    void wakeup();
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    bool hasChannel(Channel* channel);

    void assertInLoopThread(){
        if(!isInLoopThread()){
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const {
        return threadId_ == CurrentThread::tid();
    }

    bool eventHandling() const {
        return eventHandling_;
    }

    void setContext(const boost::any& context){
        context_ = context;
    }

    const boost::any& getContext() const{
        return context_;
    }

    boost::any* getMutableContext() {
        return &context_;
    }

    static EventLoop* getEventLoopOfCurrentThread();

private:
    void abortNotInLoopThread();
    void handleRead();
    void doPendingFunctors();
    void printActiveChannels() const;
    typedef std::vector<Channel*> ChannelList;
    bool looping_;                  //atomic
    std::atomic<bool> quit_;    
    bool eventHandling_;            // atomic
    bool callingPendingFunctors_;   //atomic
    int64_t iteration_;
    const pid_t threadId_;
    Timestamp pollReturnTime_;
    std::unique_ptr<Poller> poller_;
    std::unique_ptr<TimerQueue> timerQueue_;
    int wakeupFd_;
    // unlike in TimerQueue, which is internal class;
    // we don't expose channel to client
    std::unique_ptr<Channel> wakeupChannel_;
    boost::any context_;
    // scratch variable 
    ChannelList activeChannels_;
    Channel* currentActiveChannel_;
    mutable Mutex mutex_;
    std::vector<Functor> pendingFunctors_;      // GUARDED_BY(mutex_);
};
}
}
