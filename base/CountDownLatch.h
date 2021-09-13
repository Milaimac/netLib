#pragma once
// #include "Mutex.h" redefine
#include "noncopyable.h"
#include "Condition.h"


// CountDownLatch作用是确保Thread中传进去的func真的启动后，外层的start()才返回
namespace netLib{
class CountDownLatch:noncopyable{
public:
    explicit CountDownLatch(int count);

    void wait();

    void countDown();
private:
    mutable Mutex mutex_;
    Condition condition_;
    int count_;
};
}