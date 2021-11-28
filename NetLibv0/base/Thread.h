#pragma once
#include<functional>
#include<pthread.h>
#include<syscall.h>
#include <string>
// #include"netLib/base/CountDownLatch.h"
#include "Atomic.h"
#include "CountDownLatch.h"
#include "noncopyable.h"
// #include"netLib/base/noncopyable.h"

namespace netLib{

class Thread:noncopyable{
public:
    typedef std::function<void()> ThreadFunc;
    explicit Thread(ThreadFunc func, const std::string& n);  
    ~Thread();
    void start();
    int join();
    bool start() const {return started_;}
    pid_t tid(){return tid_;};
    const std::string& name() {return name_;} 
private:
    void setDefaultName();
    ThreadFunc func_;
    bool started_;
    bool joined_;
    pthread_t pthreadId_;
    pid_t tid_;
    std::string name_;
    CountDownLatch latch_;

    // static AtomicInt32 numCreated_;
    // static detail::AtomicInt32 numCreated_;
    static AtomicInt32 numCreated_;
};
}

// pthread_t 和 tid_t 
// pthread_t 是线程的标识符，但是其类型不确定，可能是结构体，或者是指针
// pit_t 其值通常是一个小整数