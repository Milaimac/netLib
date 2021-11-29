// #include "netLib/base/Thread.h"
// #include"netLib/base/CurrentThread.h"
// #include"netLib/base/Atomic.h"
#include "../base/Thread.h"
#include "../base/CurrentThread.h"
#include "../base/Exception.h"
#include "../base/Logging.h"

#include <type_traits>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

namespace netLib
{

    namespace detail
    {

        pid_t gettid()
        {
            return static_cast<pid_t>(::syscall(SYS_gettid));
        }
        void afterFork()
        {
            netLib::CurrentThread::t_cachedTid = 0;
            netLib::CurrentThread::t_threadName = "main";
            CurrentThread::tid();
        }

        class ThreadNameInitializer
        {
        public:
            ThreadNameInitializer()
            {
                netLib::CurrentThread::t_threadName = "main";
                CurrentThread::tid();
                pthread_atfork(NULL, NULL, &afterFork);
            }
        };

        ThreadNameInitializer init;
        // 为了在线程中保留name，tid这些数据
        struct ThreadData
        {
            typedef Thread::ThreadFunc ThreadFunc;
            ThreadFunc func_;
            string name_;
            pid_t *tid_;
            CountDownLatch *latch_;

            ThreadData(const ThreadFunc &Func,
                       string name,
                       pid_t *tid,
                       CountDownLatch *latch)
                : func_(std::move(Func)),
                  name_(name),
                  tid_(tid),
                  latch_(latch)
            {
            }

            void runInThread()
            {
                *tid_ = netLib::CurrentThread::tid(); //use tid not cachetid
                tid_ = NULL;
                latch_->countDown();
                latch_ = NULL;

                netLib::CurrentThread::t_threadName = name_.empty() ? "Thread" : name_.c_str();
                ::prctl(PR_SET_NAME, netLib::CurrentThread::t_threadName);
                try{
                    func_();
                    netLib::CurrentThread::t_threadName = "finished";
                }
                catch (const Exception& ex){
                    netLib::CurrentThread::t_threadName = "crashed";
                    fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
                    fprintf(stderr, "stack trace: %s\n", ex.what());
                    fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
                    abort();
                }
                catch(const std::exception& ex){
                    netLib::CurrentThread::t_threadName = "crashed";
                    fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
                    fprintf(stderr, "stack trace: %s\n", ex.what());
                    abort();
                }
                catch(...)
                {
                    netLib::CurrentThread::t_threadName = "crashed";
                    fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
                    throw ;
                }
                //TODO it can add the exception check
            }
        };

        void* startThread(void *obj)
        {
            ThreadData *data = static_cast<ThreadData *>(obj);
            data->runInThread();
            delete data;
            return NULL;
        }
    } //namespace detail

    void netLib::CurrentThread::cacheTid()
    {
        if (t_cachedTid == 0)
        {
            t_cachedTid = detail::gettid();
            // 将t_cachedTid得到tid，输入到t_tidString,snprintf返回实际的大小
            t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
        }
    }
    bool CurrentThread::isMainThread(){
        return tid() == ::getpid();
    }

    void CurrentThread::sleepUsec(int64_t usec){
        struct timespec ts = {0, 0};
        ts.tv_sec = static_cast<time_t> (usec / Timestamp::kMicroSecondsPerSecond);
        ts.tv_nsec = static_cast<long> (usec % Timestamp::kMicroSecondsPerSecond * 1000);
        ::nanosleep(&ts, NULL);
    }
    AtomicInt32 Thread::numCreated_;

    Thread::Thread(ThreadFunc func, const std::string &n)
        : started_(false),
          joined_(false),
          pthreadId_(0),
          tid_(0),
          func_(std::move(func)),
          name_(n),
          latch_(1)
    {
        setDefaultName();
    }

    Thread::~Thread()
    {
        if (started_ && !joined_)
        {
            pthread_detach(pthreadId_);
        }
    }

    void Thread::setDefaultName()
    {
        int num = numCreated_.incrementAndGet();
        if (name_.empty())
        {
            char buf[32];
            snprintf(buf, sizeof buf, "Thread%d", num);
            name_ = buf;
        }
    }

    // 这里应该就是latch的作用，start()内创建ThreadData
    // 而其中Thread也有latch_,初始化默认为1，如果创建pthread失败就会deleteThreaddata，否则执行lathc_.wait()
    // 等待latch_.countDown()， countDown会调用notifyAll()，唤醒阻塞线程，而latch_又是由startThread-->runThread()调用的
    void Thread::start()
    {
        assert(!started_);
        started_ = true;

        detail::ThreadData *data = new detail::ThreadData(func_, name_, &tid_, &latch_);
        // pthread_create success return 0, no success return errno number
        if (pthread_create(&pthreadId_, NULL, &detail::startThread, data))
        {
            started_ = false;
            delete data;
            LOG_SYSFATAL << "Failed in pthread create";
        }
        else
        {   // 创建成功， wait一下，直到真正实现到runThread的latch_.countDown()，这里就notifyAll，唤醒线程
            latch_.wait();
            assert(tid_ > 0);
        }
    }
    int Thread::join()
    {
        assert(started_);
        assert(!joined_);
        joined_ = true;
        return pthread_join(pthreadId_, NULL);
    }
} //namespace netLib
