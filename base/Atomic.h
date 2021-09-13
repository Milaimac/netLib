#ifndef NETLIB_BASE_ATOMIC_H
#define NETLIB_BASE_ATOMIC_H
#include"netLib/base/noncopyable.h"
#include<stdint.h>
namespace netLib{

namespace detail{

template<typename T>
class AtomicIntegerT : noncopyable{
public:
    AtomicIntegerT()
    :value_(0)
    {

    }

    T get()
    {   //__sync_val_compare_and_swap 也叫CAS
        return __sync_val_compare_and_swap(&value_,0,0);
    }

    T getAndAdd(T x){
        return __sync_fetch_and_add(&value_, x);
    }

    T addAndGet(T x){
        return getAndAdd(x) + x;
    }

    T incrementAndGet(){
        return addAndGet(1);
    }

    T decrementAndGet(){
        return addAndGet(-1);
    }

    void add(T x){
        getAndAdd(x);
    }

    void increment(){
        incrementAndGet();
    }

    void decrement(){
        decrementAndGet();
    }

    T getAndSet(T newValue){
        return __sync_lock_test_and_set(&value_, newValue);
    }
private:
    volatile T value_;
};
} // namespace detail
// 实现了AtomicInt32, 和AtomicInt64（也就是原子操作的整数）
typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
typedef detail::AtomicIntegerT<int64_t> AtomicINt64;
}


#endif