#include<pthread.h>
#include<cstdio>
#include"noncopyable.h"

namespace netLib{
class Mutex:noncopyable{
    
public:
    Mutex(){
        pthread_mutex_init(&mutex, NULL);
    }

    ~Mutex(){
        pthread_mutex_lock(&mutex);
        pthread_mutex_destroy(&mutex);
    }

    void lock(){
        pthread_mutex_lock(&mutex);
    }

    void unlock(){
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_t *get(){ //返回原始的mutex（未封装过的）
        return &mutex;
    }
private:
    pthread_mutex_t mutex;

private:
    friend class Condition;
};

class MutexLockGuard:noncopyable{
public:
    explicit MutexLockGuard(Mutex &_mutex):mutex(_mutex){
        mutex.lock();
    }
    ~MutexLockGuard(){
        mutex.unlock();
    }
private:
    Mutex& mutex;
};
}