#pragma once
#include<errno.h>
#include<time.h>
#include "Mutex.h"
#include "noncopyable.h"
// #include"netLib/base/Mutex.h"
// #include"netLib/base/noncopyable.h"
#include<pthread.h>

namespace netLib{
class Condition:noncopyable{
public:
    explicit Condition(Mutex& mutex_):mutex(mutex_){
        pthread_cond_init(&cond, NULL);
    }

    ~Condition(){
        pthread_cond_destroy(&cond);
    }
    void wait(){
        pthread_cond_wait(&cond, mutex.get());
    }
    void notify(){
        pthread_cond_signal(&cond);
    }
    void notifyAll(){
        pthread_cond_broadcast(&cond);
    }

    bool waitForSeconds(int seconds){
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME,&abstime);         //clock_gettime 可以得到纳秒级别的定时
        abstime.tv_sec += static_cast<time_t> (seconds);
        return ETIMEDOUT == pthread_cond_timedwait(&cond, mutex.get(), &abstime);
    }

private:
    Mutex &mutex;
    pthread_cond_t cond;
};
}