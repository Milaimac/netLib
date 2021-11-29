# 先是对Timerfd_create做简单的介绍
```c
int timerfd_create(int clockid, int flags);
```
`timerfd_create` 函数将时间变成了一个文件描述符， 该文件描述符在定时器超时的那一刻变得可读，这样就方便融入到select 和 poll中去

timerfd_create() 创建一个新的timer对象，返回一个文件描述符对应于这个timer. 

    clockid参数指定时钟，用于标志timer的进度，它必须是下面的一个：

    CLOCK_REALTIME

    一个可设置的系统范围的实时时钟。

    CLOCK_MONOTONIC
    一个不可设置的单调递增的时钟，从过去未指定的某个时间点开始测量时间，时间在系统启动后不会再改变。
    
    
    flags参数：

    TFD_NONBLOCK   ： 置位O_NONBLOCK参数

    TFD_CLOEXEC       ： 置位O_CLOEXEC 参数

```cpp
int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
/*
    timerfd_settime()此函数用于设置新的超时时间，并开始计时,能够启动和停止定时器;
    fd: 参数fd是timerfd_create函数返回的文件句柄
    flags：参数flags为1代表设置的是绝对时间（TFD_TIMER_ABSTIME 表示绝对定时器）；为0代表相对时间。
    new_value: 参数new_value指定定时器的超时时间以及超时间隔时间
    old_value: 如果old_value不为NULL, old_vlaue返回之前定时器设置的超时时间，具体参考timerfd_gettime()函数
    
    ** it_interval不为0则表示是周期性定时器。
       it_value和it_interval都为0表示停止定时器
*/
```


## TimerQueue的工作机制

为了避免空闲连接持续占用资源， 使用time wheel 踢掉空闲连接. muduo给的example示例是使用boost::circular_buffer， 通过加入新连接，将空闲的连接自动踢掉。  并且使用weak_ptr来判断Entry（连接）是否是真的释放。

而muduo最后实现的是直接依赖expiration


**detail命名空间中**

~~~cpp
int createTimerfd(); 
// timerfd_create 通过将时间变为文件描述符，该文件描述符在定时器超时时，会变得可读
struct timespec howMuchTimeFromNow(Timestamp when)
// 判断when 与现在还差了多少 时间
void readTimerfd(int timerfd, Timestamp now);
// 如果timerfd超时了，就会变得可读； 从而可以执行::read(timerfd, &howmany, sizeof(howmany));
void resetTimerfd(int timerfd, Timestamp expiration)
// 设置新的超时时间
~~~

主要功能函数 addTimer  

在`timers_ `和 `activeTimers_` insert 对应`Entry` 和`ActiveTimer`

```cpp
TimerId TimerQueue::addTimer(TimerCallback cb,
                                Timestamp when,
                                double interval)
// 添加Timer， 并且让事件循环 loop_->runInLoop(std::bind(&TimerQueue::addTimeInLoop), this, timer);

void TimerQueue::addTimerInLoop(Timer* timer)
{
    loop_->assertInLoopThread();
    bool earliestChanged = insert(timer);

    if (earliestChanged)
    {
        resetTimerfd(timerfd_, timer->expiration());
    }
}

bool TimerQueue::insert(Timer* timer)
{
    loop_->assertInLoopThread();
    assert(timers_.size() == activeTimers_.size());
    bool earliestChanged = false;
    Timestamp when = timer->expiration();
    TimerList::iterator it = timers_.begin();
    if (it == timers_.end() || when < it->first)
    {
        earliestChanged = true;
    }
    {
        std::pair<TimerList::iterator, bool> result
        = timers_.insert(Entry(when, timer));
        assert(result.second); (void)result;
    }
    {
        std::pair<ActiveTimerSet::iterator, bool> result
        = activeTimers_.insert(ActiveTimer(timer, timer->sequence()));
        assert(result.second); (void)result;
    }

    assert(timers_.size() == activeTimers_.size());
    return earliestChanged;
}

```

