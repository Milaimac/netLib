#include "../net/EventLoopThread.h"

#include "../net/EventLoop.h"

using namespace netLib;
using namespace netLib::net;

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb, 
                                const std::string& name)
    :loop_(nullptr),
    exiting_(false),
    thread_(std::bind(&EventLoopThread::threadFunc, this), name),
    mutex_(),
    cond_(mutex_),
    callback_(cb)
{
}

EventLoopThread::~EventLoopThread()
{
    exiting_ = true;
    if(loop_ != nullptr)    //not 100% race - free, threadFunc cound be running callback_
    {
        // still a tiny chance to call destructed object, if threadFunc exits just now.
        // but when EventLoopThread destructs, usually programming is exiting anyway.
        loop_->quit();
        thread_.join();    
    }
}

EventLoop* EventLoopThread::startLoop()
{
    assert(!thread_.started()); 
    thread_.start();

    EventLoop* loop = nullptr;
    
    {
    MutexLockGuard lock(mutex_);
    while(loop_ == nullptr)
    {
        cond_.wait();
    }
    loop = loop_;
    }
    return loop;
}


// If ThreadInitCallback exits, do it in the loop;
// then notify the mutex_ that cond_ map to, actually notify the cond_.wait() in the startLoop, and the callback_ will init the loop ?
// do the EventLoop
void EventLoopThread::threadFunc()
{
    EventLoop loop;
    if(callback_)
    {
        callback_(&loop);
    }

    {
    MutexLockGuard lock(mutex_);
    loop_ = &loop;
    cond_.notify();
    }

    loop.loop();
    // assert(exiting_)
    MutexLockGuard lock(mutex_);
    loop_ = nullptr;
    
}

