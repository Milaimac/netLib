#ifndef NETLIB_BASE_CURRENTTHREAD_H
#define NETLIB_BASE_CURRENTTHREAD_H
#include<stdint.h>
#include<string>
namespace netLib{
    using namespace std;
namespace CurrentThread{
    //__thread 是gcc编译器内置的线程局部存储设施，存储效率等价于全局变量
    //__thread变量在每一个线程都由一份独立实例 ， 各线程值是互不干扰的
    // 限制：只能修饰POD类型
    // internal
    extern __thread int t_cachedTid;
    extern __thread char t_tidString[32];
    extern __thread int t_tidStringLength;
    extern __thread const char* threadName;

    void cacheTid();

    inline int tid()
    {
        // __builtin_expect(EXP, N) 告诉编译器 EXP == N 的可能性很大
        if (__builtin_expect(t_cachedTid == 0, 0))
        {
        cacheTid();
        }
        return t_cachedTid;
    }

    inline const char* tidString(){
        return t_tidString;
    }

    inline int tidStringLength(){
        return t_tidStringLength;
    }

    inline const char* name(){
        return threadName;
    }

    bool isMainThread();

    void sleepUsec(int64_t usec);

    string stackTrace(bool demangle);
    //mangle 将C++源程序标识符转化为C++ ABI标识符， demangle就是反过来
};
}

#endif