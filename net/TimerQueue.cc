#include "../net/TimerQueue.h"

#include "../base/Logging.h"
#include "../net/EventLoop.h"
#include "../net/Timer.h"
#include "../net/TimerId.h"

#include <sys/timerfd.h>
#include <unistd.h>
namespace netLib{
namespace net{
namespace detail{
int createTimerfd(){
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, 
                                TFD_NONBLOCK | TFD_CLOEXEC);
    if(timerfd < 0){
        LOG_SYSFATAL << "Failed in timerfd_create";
    }
    return timerfd;   
}

struct timespec howMuchTimeFromNow(Timestamp when){
    int64_t microseconds = when.microSecondsSinceEpoch() 
                            - Timestamp::now().microSecondsSinceEpoch();
    if(microseconds < 100){
        microseconds = 100;
    }

    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(microseconds / Timestamp::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long> ((microseconds % Timestamp::kMicroSecondsPerSecond) * 1000) ;
    return ts;
}
// readTimerfd will judge the over clock by read
void readTimerfd(int timerfd, Timestamp now){
    uint64_t howmany;
    ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
    LOG_TRACE << "TimerQueue::handleRead() " << howmany << "at" << now.toString();
    if(n != sizeof howmany){
        LOG_ERROR << "TimerQueue::handleRead() reads" << n << "bytes instead of 8";
    }
}
// 设置新的超时事件
void resetTimerfd(int timerfd, Timestamp expiration){
    struct itimerspec newValue;
    struct itimerspec oldValue;
    memZero(&newValue, sizeof newValue);
    memZero(&oldValue, sizeof oldValue);
    newValue.it_value = howMuchTimeFromNow(expiration);
    int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
    if(ret){
        LOG_SYSERROR << "timerfd_settimer()"; 
    }
}

} //namespace detail  
} //namespace net 
} //namespace netLib


using namespace netLib;
using namespace netLib::net;
using namespace netLib::net::detail;
TimerQueue::TimerQueue(EventLoop* loop)
    :loop_(loop), 
    timerfd_(createTimerfd()),
    timerfdChannel_(loop, timerfd_),
    timers_(),
    callingExpiredTimers_(false)
{
    timerfdChannel_.setReadCallback(
        std::bind(&TimerQueue::handleRead, this)
    );
    timerfdChannel_.enableReading();
}
TimerQueue::~TimerQueue(){
    timerfdChannel_.disableAll();
    timerfdChannel_.remove();
    ::close(timerfd_);
    // do not remove channel since we're in EventLoop::dtor()
    for(const Entry& timer:timers_){
        delete timer.second;
    }
}

TimerId TimerQueue::addTimer(TimerCallback cb,
                                Timestamp when,
                                double interval)
{
    Timer* timer = new Timer(std::move(cb), when, interval);
    loop_->runInLoop(
        std::bind(&TimerQueue::addTimerInLoop, this, timer)
    );
    return TimerId(timer, timer->sequence());
}

void TimerQueue::cancel(TimerId timerId){
    loop_->runInLoop(
        std::bind(&TimerQueue::cancelInLoop, this, timerId)
    );
}

void TimerQueue::addTimerInLoop(Timer* timer){
    loop_->assertInLoopThread();
    bool earilestChanged = insert(timer);

    if(earilestChanged){
        resetTimerfd(timerfd_, timer->expiration());
    }
}
// Entry is std::pair<Timestamp, Timer>
void TimerQueue::cancelInLoop(TimerId timerId){
    loop_->assertInLoopThread();
    assert(timers_.size() == activeTimers_.size());
    ActiveTimer timer(timerId.timer_, timerId.sequence_);
    ActiveTimerSet::iterator it = activeTimers_.find(timer);
    if(it != activeTimers_.end()){
        size_t n = timers_.erase(Entry(it->first->expiration(), it->first));
        assert(n == 1); (void)n;
        delete it->first;
        activeTimers_.erase(it);
    }
    else if(callingExpiredTimers_){     // 如果要被删除的TimerId不在activeTimers中，则将之放入cancelingTimers_, 并且handleRead 会设置callingExpiredTimers_为true
        cancelingTimers_.insert(timer);
    }
    assert(timers_.size() == activeTimers_.size());
}

void TimerQueue::handleRead(){
    loop_->assertInLoopThread();
    Timestamp now(Timestamp::now());
    readTimerfd(timerfd_, now);

    std::vector<Entry> expired = getExpired(now);

    callingExpiredTimers_ = true;
    cancelingTimers_.clear();
    // 调用回调函数
    for(const Entry &it:expired){
        it.second->run();
    }
    callingExpiredTimers_ = false;
    // if the timer is repeat, it will restart, or cancel
    reset(expired, now);
}   

// 如果到任期， 先是将activeTimers_中对应的timer移出， 在handleRead中，执行expired中timer的callback事件
std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now){
    assert(timers_.size() == activeTimers_.size());
    std::vector<Entry> expired;
    Entry sentry(now, reinterpret_cast<Timer*> (UINTPTR_MAX));  // UINIPTR_MAX 足够表示对象指针的极限
    TimerList::iterator end = timers_.lower_bound(sentry);
    assert(end == timers_.end() || now < end->first);
    std::copy(timers_.begin(), end, back_inserter(expired));
    timers_.erase(timers_.begin(), end);

    for(const Entry& it:expired){
        ActiveTimer timer(it.second, it.second->sequence());
        size_t n = activeTimers_.erase(timer);
        assert(n == 1);
        (void)n;
    }
    assert(timers_.size() == activeTimers_.size());
    return expired;
}

// reset 
// if Timer is repeat && timer is not in cancelingTimers_ , so restart it and insert into timers_;
// else delete it.second (Timer*)
void TimerQueue::reset(const std::vector<Entry>& expired, Timestamp now){
    Timestamp nextExpire;

    for(const Entry& it:expired){
        ActiveTimer timer(it.second, it.second->sequence());
        if(it.second->repeat()
            && cancelingTimers_.find(timer) == cancelingTimers_.end())
        {
            it.second->restart(now);
            insert(it.second);
        }
        else
        {
            delete it.second;
        }
    }
    if(!timers_.empty()){
        nextExpire = timers_.begin()->second->expiration();
    }
    if(nextExpire.valid()){
        resetTimerfd(timerfd_, nextExpire);
    }
}

bool TimerQueue::insert(Timer* timer)
{
    loop_->assertInLoopThread();
    assert(timers_.size() == activeTimers_.size());
    bool earliestChanged = false;
    Timestamp when = timer->expiration();
    TimerList::iterator it = timers_.begin();
    if(it == timers_.end() || when < it->first)
    {
        earliestChanged = true;
    }
    {
        // set insert后，会返回位置信息和bool 表示是否成功插入
        std::pair<TimerList::iterator, bool> result = timers_.insert(Entry( when, timer));
        assert(result.second);
        (void) result;
    }
    {
        std::pair<ActiveTimerSet::iterator, bool> result = activeTimers_.insert(ActiveTimer(timer, timer->sequence()));
        assert(result.second);
        (void) result;
    }
    return earliestChanged;
}