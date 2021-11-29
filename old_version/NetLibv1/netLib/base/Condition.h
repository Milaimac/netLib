#pragma once
#include "Mutex.h"
#include <pthread.h>

namespace netLib
{
    class Condition : noncopyable
    {
    public:
        explicit Condition(Mutex &mutex)
            : mutex_(mutex)
        {
            // pthread_cond_init(&pcond_, NULL);
            MCHECK(pthread_cond_init(&pcond_, NULL));
        }

        ~Condition()
        {
            // pthread_cond_destroy(&pcond_);
            MCHECK(pthread_cond_destroy(&pcond_));
        }
        void wait()
        {
            // pthread_cond_wait(&pcond_, mutex.get());
            Mutex::UnassignGuard ug(mutex_);
            MCHECK(pthread_cond_wait(&pcond_, mutex_.getPthreadMutex()));
        }
        void notify()
        {
            // pthread_cond_signal(&pcond_);
            MCHECK(pthread_cond_signal(&pcond_));
        }
        void notifyAll()
        {
            // pthread_cond_broadcast(&pcond_);
            MCHECK(pthread_cond_broadcast(&pcond_));
        }

        bool waitForSeconds(double seconds)
        {
            struct timespec abstime;
            clock_gettime(CLOCK_REALTIME, &abstime); //clock_gettime 可以得到纳秒级别的定时

            const int64_t kNanoSecondsPerSecond = 1000000000; //10e9
            int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);
            
            abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
            abstime.tv_nsec = static_cast<long> ((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);
            
            Mutex::UnassignGuard ug(mutex_);
            return ETIMEDOUT == pthread_cond_timedwait(&pcond_, mutex_.getPthreadMutex(), &abstime);
        }

    private:
        Mutex &mutex_;
        pthread_cond_t pcond_;
    };
}