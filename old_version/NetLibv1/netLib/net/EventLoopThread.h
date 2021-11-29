#pragma once

#include "../base/Condition.h"
#include "../base/Mutex.h"
#include "../base/Thread.h"

// EventLoopThread
// 

namespace netLib
{
namespace net
{
class EventLoop;

class EventLoopThread: noncopyable
{
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;

    EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(), 
                    const std::string& name = std::string());
    ~EventLoopThread();

    EventLoop* startLoop();
private:
    void threadFunc();

    EventLoop* loop_;       // guard by mutex
    bool exiting_;
    Thread thread_;         
    Mutex mutex_;
    Condition cond_;        // guard by mutex
    ThreadInitCallback callback_;
};
}
} // namespace netLib
