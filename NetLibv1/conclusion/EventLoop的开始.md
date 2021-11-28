# EventLoop的实现
muduo 的EventLoop实现了什么？

实现函数
```cpp
// construct and destruct 
EventLoop();
~EventLoop();

void loop();
void quit();

Timestamp poolReturntime() const {return pollreturnTime_;}
int64_t iteration() const {return iteration_;}
void runInLoop(Functor cb);
void queueInLoop(Functor cb);
size_t queueSize() const;
TimerId runAt(Timestamp time, TimerCallback cb);
TimerId runAfter(double delay, TimerCallback cb);
TimerId runEvery(double interval, TimeCallback cb);

void cancel(TimerId timerId);
void wakeup();
void updateChannle(Channle* channle);
void removeChannle(Channle* channle);
bool hasChannle(Channle* channle);

void assertInloopThread(){
    if(!isInLoopThread()){
        abortNotInLoopThread();
    }
}

bool isInLoopThread() const{
    return threadId == CurrentThead::tid();
}

bool eventHandling() const{
    return eventHanding_;
}
void setContex(const boost::any& context){
    context_ = context;
}

```
数据成员

```cpp
typedef std::vector<Channel*> ChannelList;
bool looping_;
std::atomic<bool> quit_;
bool eventHandling_;
bool callingPendingFunctors_;
int64_t iteration_;
const pid_t threadId_;
Timestamp poolReturnTime_;
std::unique_ptr<Poller> poller_;
std::unique_ptr<TimerQueue> timerQueue_;
int wakeupFd_;
std::unique_ptr<Channel> wakeupChannels_;
boost::any context_;
ChannelList activeChannels_;
Channel* currentActiveChannel_;
mutable MutexLock mutex_;   
std::vector<Functor> pendingFunctors_;
```

# 重新思考一下具体的架构


## 最核心的事件分发 -- Channel
Channle Class 

- 每个Channle对象只属于一个EventLoop，也就是只属于一个IO 线程。
- 每个Channle对象只负责一个文件描述符的IO事件分发，但是它并不拥有这个fd，析构的时候也不会关闭这个fd

--- 
muduo 通过kNoneEvent, kWriteEvent, kReadEvent和events进行操作，实现事件类型的选择, 而后通过poller对revents进行操作？

~~~cpp
// muduo 设置events_
int fd() const { return fd_; }
int events() const { return events_; }
void set_revents(int revt) { revents_ = revt; } // used by pollers
// int revents() const { return revents_; }
bool isNoneEvent() const { return events_ == kNoneEvent; }
// 通过设置kNoneEvent, kReadEvent, kWriteEvent， 达到设置读事件， 写事件等
void enableReading() { events_ |= kReadEvent; update(); }
void disableReading() { events_ &= ~kReadEvent; update(); }
void enableWriting() { events_ |= kWriteEvent; update(); }
void disableWriting() { events_ &= ~kWriteEvent; update(); }
void disableAll() { events_ = kNoneEvent; update(); }
bool isWriting() const { return events_ & kWriteEvent; }
bool isReading() const { return events_ & kReadEvent; }
~~~

### muduo的设置回调函数

~~~cpp
// 设置回调函数
void handleEvent(Timestamp receiveTime);
void setReadCallback(ReadEventCallback cb)
{ readCallback_ = std::move(cb); }
void setWriteCallback(EventCallback cb)
{ writeCallback_ = std::move(cb); }
void setCloseCallback(EventCallback cb)
{ closeCallback_ = std::move(cb); }
void setErrorCallback(EventCallback cb)
{ errorCallback_ = std::move(cb); }
~~~


~~~cpp
// 设置回调函数
void setReadHandler(CallBack &&readHandler) { readHandler_ = readHandler; }
void setWriteHandler(CallBack &&writeHandler) {
writeHandler_ = writeHandler;
}
void setErrorHandler(CallBack &&errorHandler) {
errorHandler_ = errorHandler;
}
void setConnHandler(CallBack &&connHandler) { connHandler_ = connHandler; }

~~~


## 通过判断revets 设置回调函数
~~~cpp


// 实现事件分发的函数
// 通过判断revents_ 来调用不同的回调函数
void Channel::handleEventWithGuard(Timestamp receiveTime)
{
  eventHandling_ = true;
  LOG_TRACE << reventsToString();
  if ((revents_ & POLLHUP) && !(revents_ & POLLIN))
  {
    if (logHup_)
    {
      LOG_WARN << "fd = " << fd_ << " Channel::handle_event() POLLHUP";
    }
    if (closeCallback_) closeCallback_();
  }

  if (revents_ & POLLNVAL)
  {
    LOG_WARN << "fd = " << fd_ << " Channel::handle_event() POLLNVAL";
  }

  if (revents_ & (POLLERR | POLLNVAL))
  {
    if (errorCallback_) errorCallback_();
  }
  if (revents_ & (POLLIN | POLLPRI | POLLRDHUP))
  {
    if (readCallback_) readCallback_(receiveTime);
  }
  if (revents_ & POLLOUT)
  {
    if (writeCallback_) writeCallback_();
  }
  eventHandling_ = false;
}
~~~

~~~cpp
// 和muduo的handleEventsWithGuard 具有相同的职能
void handleEvents() {
events_ = 0;
if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {
    events_ = 0;
    return;
}
if (revents_ & EPOLLERR) {
    if (errorHandler_) errorHandler_();
    events_ = 0;
    return;
}
if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
    handleRead();
}
if (revents_ & EPOLLOUT) {
    handleWrite();
}
handleConn();
}
~~~

--- 

看到这里，可能有疑问，**什么时候对revents_进行操作的**

在EventLoop的loop函数中，有个操作
```cpp
pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
```

通过EpollPoller将activeChannels_给填好。 

其中poll操作通过调用EpollPoller中的events_ (epoll_event 结构体) 实现 `epoll_wait(epollfd, &*events_.begin(), static_cast<int>(events_.size()), timeoutMs)`

而后通过fillActiveChannels实现填好Channels的操作

---

回忆一下epoll_events的结构体

epoll_event的epoll_data 有两个最重要的东西， ptr和fd。

- ptr是用来执行用户自定义的结构体，在这里我们就用这个指针执行了Channel对象；
- 而fd 就是使用这个事件的文件描述符

```cpp
struct epoll_event{
    uint32_t events;
    epoll_data  data;
};

typedef union epoll_data{
    void *ptr;
    int    fd;
    uint32_t u32;
    uint64_t u64;
}epoll_data_t;
```
---
```cpp
void EPollPoller::fillActiveChannels(int numEvents,
                                     ChannelList* activeChannels) const
{
  assert(implicit_cast<size_t>(numEvents) <= events_.size());
  for (int i = 0; i < numEvents; ++i)
  {
    Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
#ifndef NDEBUG
    int fd = channel->fd();
    ChannelMap::const_iterator it = channels_.find(fd);
    assert(it != channels_.end());
    assert(it->second == channel);
#endif
    // 最后在这里设置revents_
    channel->set_revents(events_[i].events);
    activeChannels->push_back(channel);
  }
}
```
在这里还有个疑问， epoll_wait将events的指针和它设定的events一并返回，那么muduo的代码什么时候对events_的ptr进行设置了？

以及update 到底有什么用
```cpp
void Channel::update(){
    addedToLoop_ = true;
    loop_->updateChannel(this);
}

void EventLoop::updateChannel(Channel* channel){
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}

// kNew和kDelted到底有什么用。 
void EpollPoller::updateChannel(Channel* channel){
    Poller::assertInLoopTheaed();
    const int index = channel->index();     //这里就是poller使用index的地方， 具体有什么用处
    LOG_TRACE << "fd = " << channel->fd()
    << " events = " << channel->events() << " index = " << index;
    if(index == kNew || index == kDeleted){
        int fd = channel->fd();
        // a new one, add with EPOLL_CTL_ADD
        if(index == kNew){
            assert(channels_.find(fd)==channels_.end());
            channels_[fd] = channel;
        }
        else{ //index == kDeleted
            assert(channels_.find(fd)!=channels_.end());
            assert(channels_[fd]== channel);
        }
        channel->set_index(kAdded);
        update(EPOLL_CTL_ADD, channel);
    }
    else{
        // update existing one with EPOLL_CTL_MODE/DEL
        int fd = channel->fd();
        (void)fd;
        assert(channels_.find(fd) != channels_.end());
        assert(channels_[fd] == channel);
        assert(index == kAdded);
        if(channel->isNoneEvent());
        {
            update(EPOLL_CTL_DEL, channel);
            channel->set_index(kDelted);
        }
        else{
            update(EPOLL_CTL_MOD, channel);
        }
    }

}

```


```cpp
Timestamp EPollPoller::poll(int timeoutMs, ChannelList* activeChannels)
{
  LOG_TRACE << "fd total count " << channels_.size();
  int numEvents = ::epoll_wait(epollfd_,
                               &*events_.begin(),
                               static_cast<int>(events_.size()),
                               timeoutMs);
  int savedErrno = errno;
  Timestamp now(Timestamp::now());
  if (numEvents > 0)
  {
    LOG_TRACE << numEvents << " events happened";
    fillActiveChannels(numEvents, activeChannels);
    if (implicit_cast<size_t>(numEvents) == events_.size())
    {
      events_.resize(events_.size()*2);
    }
  }
  else if (numEvents == 0)
  {
    LOG_TRACE << "nothing happened";
  }
  else
  {
    // error happens, log uncommon ones
    if (savedErrno != EINTR)
    {
      errno = savedErrno;
      LOG_SYSERR << "EPollPoller::poll()";
    }
  }
  return now;
}
```

