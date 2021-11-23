#pragma once

#include <set>
#include <vector>

#include "../base/Mutex.h"
#include "../base/Timestamp.h"
#include "../net/Callbacks.h"
#include "../net/Channel.h"

namespace netLib{
namespace net{
    
class EventLoop;
class Timer;
class TimerId;

class TimerQueue : noncopyable{
public:
    explicit TimerQueue(EventLoop* loop);
    
    ~TimerQueue();

    TimerId addTimer(TimerCallback cb,
                    Timestamp when,
                    double interval);

    void cancel(TimerId timerId);
private:
    // there is the Timestamp and Timer; and Timestamp is the Timer's expiration
    typedef std::pair<Timestamp, Timer*> Entry;
    typedef std::set<Entry> TimerList;
    // here ActiveTimer is the Timer and TimerId(seqence)  
    typedef std::pair<Timer*, int64_t> ActiveTimer;
    typedef std::set<ActiveTimer> ActiveTimerSet;

    void addTimerInLoop(Timer* timer);
    void cancelInLoop(TimerId timerId);
    void handleRead();
    std::vector<Entry> getExpired(Timestamp now);
    void reset(const std::vector<Entry>& expired, Timestamp now);
    bool insert(Timer* timer);
    EventLoop* loop_;
    const int timerfd_;
    Channel timerfdChannel_;
    TimerList timers_;
    ActiveTimerSet activeTimers_;
    bool callingExpiredTimers_;
    ActiveTimerSet cancelingTimers_;
};

} // namespace net
}