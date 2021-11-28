// #include "netLib/base/Thread.h"
// #include"netLib/base/CurrentThread.h"
// #include"netLib/base/Atomic.h"
#include "Thread.h"
#include "CurrentThread.h"
#include "Atomic.h"

#include<sys/prctl.h>       //对应prtcl设置线程名字
// #include<linux/unistd.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<assert.h>
// #include<stdint.h>
#include<errno.h>
// #include<memory>

namespace netLib{

namespace detail{

pid_t gettid(){
    return static_cast<pid_t>(::syscall(SYS_gettid));
}



// 为了在线程中保留name，tid这些数据
struct ThreadData{
    typedef Thread::ThreadFunc ThreadFunc;
    ThreadFunc func_;
    string name_;
    pid_t* tid_;
    CountDownLatch* latch_; 

    ThreadData(const ThreadFunc& Func, string name, pid_t* tid, CountDownLatch* latch)
    :func_(std::move(Func)), name_(name), tid_(tid), latch_(latch){

    }

    void runThread(){
        *tid_ = netLib::CurrentThread::tid();        //use tid not cachetid
        tid_ = NULL;
        latch_->countDown();
        latch_ = NULL;
        
        netLib::CurrentThread::t_threadName = name_.empty() ? "Thread":name_.c_str();
        prctl(PR_SET_NAME, netLib::CurrentThread::t_threadName);

        func_();

        netLib::CurrentThread::t_threadName = "Finished";
        //TODO it can add the exception check
    }
};

void* startThread(void* obj){
    ThreadData* data = static_cast<ThreadData*> (obj);
    data->runThread();
    delete(data);
    return NULL;
}
}       //namespace detail


void netLib::CurrentThread::cacheTid(){
    if(t_cachedTid == 0){
        t_cachedTid = detail::gettid();
        // 将t_cachedTid得到tid，输入到t_tidString,snprintf返回实际的大小
        t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%d ", t_cachedTid);
    }
}

AtomicInt32 Thread::numCreated_;

Thread::Thread(ThreadFunc func, const std::string& n)
:started_(false), 
joined_(false),
pthreadId_(0),
tid_(0),
name_(n),
latch_(1)
{
    setDefaultName();
}

Thread::~Thread(){
    if(started_ && !joined_){
        pthread_detach(pthreadId_);
    }
}

void Thread::setDefaultName(){
    int num = numCreated_.decrementAndGet();
    if(name_.empty()){
        char buf[32];
        snprintf(buf, sizeof buf, "Thread%d", num);
        name_ = buf;
    }
}

// 这里应该就是latch的作用，start()内创建ThreadData
// 而其中Thread也有latch_,初始化默认为1，如果创建pthread失败就会deleteThreaddata，否则执行lathc_.wait()
// 等待latch_.countDown()， countDown会调用notifyAll()，唤醒阻塞线程，而latch_又是由startThread-->runThread()调用的
void Thread::start(){
    assert(!started_);
    started_ = true;

    detail::ThreadData* data = new detail::ThreadData(func_, name_, &tid_, &latch_);
    // pthread_create success return 0, no success return errno number
    if(pthread_create(&pthreadId_, NULL, &detail::startThread, data)){
        started_ = false;
        delete(data);
        //TODO LOG_SYSFATAL << "Failed in pthread create"
        //TODO 现在还没有做日志库出来
    }
    else{// 创建成功， wait一下，直到真正实现到runThread的latch_.countDown()，这是就notifyAll，唤醒线程
        latch_.wait();
        assert(tid_ > 0);
    }
}


}       //namespace netLib