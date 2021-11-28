## static_pointer_cast


## Atomic

Gcc 4.1.2版本之后，对X86或X86_64支持内置原子操作。就是说，不需要引入第三方库（如pthread）的锁保护，即可对1、2、4、8字节的数值或指针类型，进行原子加/减/与/或/异或等操作。


将_sync_系列17个函数声明如下

1. type __sync_fetch_and_add(type \*ptr, type value, ... ) 将value加到\*ptr上面去,结果更新到\*ptr上，并返回操作之前*ptr的值
2. type __sync_fetch_and_sub(type \*ptr, type value, ... ) 从\*ptr减去value，结果更新到\*ptr上，并返回操作之前\*ptr的值
3. type __sync_fetch_and_or(type \*ptr, type value, ... ) 将\*ptr和value相或，结果更新到\*ptr上，并返回操作之前\*ptr的值
4. type __sync_fetch_and_and(type \*ptr, type vaule, ... ) 将\*ptr和value相与，结果更新到\*ptr，并返回操作之前\*ptr的值
5. type __sync_fetch_and_xor (type \*ptr, type value, ...) 将\*ptr与value异或，结果更新到\*ptr，并返回操作之前\*ptr的值
6. type __sync_fetch_and_nand (type \*ptr, type value, ...)  将\*ptr取反后，与value相与，结果更新到\*ptr，并返回操作之前\*ptr的值
7. type __sync_add_and_fetch (type \*ptr, type value, ...) 将value加到\*ptr上，结果更新到\*ptr，并返回操作之后新\*ptr的值
8. 同理 sub or and xor nand
13. bool __sync_val_compare_swap(type \*ptr, type oldval, type newval, ...) 比较\*ptr和oldval的值，如果两者相等，则将newval更新到\*ptr并返回true
14. type __sync_val_compare_and_swap (type *ptr, type oldval type newval, ...) 比较\*ptr和oldval的值，如果二者相等，则将newval更新到\*ptr，并且返回操作之前\*ptr的值
15. _sync_synchronize (...) 发出完整内存栅栏
16. type __sync_lock_test_and_set(type \*ptr, type value, ...)  将value写入 \*ptr，并且对\*ptr加锁，并且返回操作之前\*ptr的值，即tryspinlock语义
17. void __sync_lock_release(type \*ptr) 把0写入\*ptr中，并对\*ptr解锁，即unlock spinlock语义


## protcl 设置thread名
prtcl.md

## 前向声明

Accpetor.cc 使用了Event对象，但是不include  Event对象， 此时的Event对象是不完全的类型，也就是直到Event是一个类型，但是该类型的一些性质都不知道

所以不能通过这个前向声明的类的指针或者对象去操作它的成员

### 前向声明带来的限制和好处

限制条件

- 不能够定义Event的对象
- 只能够使用Event的指针和引用类型
- 用于声明返回类型和返回类型的函数

好处

1. 通过前向声明减少头文件的引入，提高编译效率

## implicit_cast 

implicit_cast 是static_cast 或 const_cast 向上转型的安全版本（将Foo指针转换为SuperClass of Foo 或者 将Foo 转化为const Foo)



在C++中有四种类型转换
1、static_cast：这个是最经常使用的类型转换。凡是C++隐式执行的类型转换都能够用static_cast显式完毕。在隐式转换时有时编译器会有警告信息。可是显示转换就不会有。

2、const_cast：从名字能够看出和const有关，这个转换的作用是去除或加入const特性，它能够将一个const变量转换为非const变量，或将一个非const变量转换为const变量。

3、dynamic_cast：这个是执行时检查该类型转换是否安全。被转换的类型必须是多态（即有虚函数）。具体可參考这里

4、interpret_cast：interpret意思为又一次解释。意思为将数据的二进制格式又一次解释，它依赖机器。

在boost库中有个implicit_cast。这个表示“隐式”转换，即能够隐式转换的地方才干够用它。

以一个样例说明,在菱形继承中。最底层的对象能够转换为中层对象
```cpp
#include<boost/implicit_cast.hpp>
#include<iostream>

class Top{};
class MiddleA:public Top{};
class MiddleB:public Top{};
class Bottom:public MiddleA,public MiddleB{};

void Function(MiddleA& A)
{
    std::cout<<"MiddleA Function"<<std::endl;
}
void Function(MiddleB& B)
{
    std::cout<<"MiddleB Function"<<std::endl;
}
int main()
{
    Bottom bottom;

    Function(bottom);
    return 0;
}
```
这时假设编译，就会出现错误。由于在调用函数Function时。bottom既能够默认转换为MiddleA，也能够默认转换为MiddleB，假设不明白指出就会出现歧义。这时能够改为：
```c
Function(static_cast<MiddleA&>(bottom));
```
或
```c
Function(static_cast<MiddleB&>(bottom));
```
程序能够执行了。可是假设不小心这样使用了：
```c
Top top;
Function(static_cast<MiddleB&>top);
```
这样编译能够通过，可是在执行时能够崩溃。由于top不是“一种”MiddleB。可是编译器不能发现这个问题。这时假设用implicit_cast就不会有这个问题了，在编译时就会给出错误信息。

在转换时up-cast不会有问题，由于派上类就是一种基类。它包括基类全部信息。可是基类不是一种派上类。假设down-cast就可能有问题。

static_cast比較强大，能够执行上述两种转换。

可是implicit_cast比較弱一点，仅仅能执行up-cast。

## cxa_demangle 

将ABI标识符变为C++标识符    

## poll函数

避免值-结果参数


当往一个套接字函数传递一个套接字地址结构时，该结构总是以引用形式来传递，也就是说传递的是指向该结构的一个指针。

该结构的长度也作为一个参数来传递，不过其传递方式取决于该结构的传递方向：进程->内核    内核->进程

1）进程->内核 传递套接字地址结构的函数有3个：bind、connect、sendto

这些函数的一个参数是指向某个套接字地址结构的指针，另一个参数是该结构的整数大小

2）内核->进程 传递套接字地址结构的函数有4个：accept、recvfrom、getsockname、getpeername

这4个函数的其中2个参数是指向某个套接字地址结构的指针和指向表示该结构大小的整数变量的指针

 

把套接字地址结构大小这个参数从一个整数改变为指向某个整数变量的指针，其原因：

当函数被调用时，结构大小是一个值（value），它告诉内核该结构的大小，这样内核在写该结构时不至于越界;

当函数返回时，结构大小又是一个结果（result），它告诉进程内核在该结构体究竟存储了多少信息。

 

 

这种类型的参数称为：值-结果参数


## timerfd_create

timerfd是Linux为用户程序提供的一个定时器接口。这个接口基于文件描述符，通过文件描述符的可读事件进行超时通知，所以能够被用于select/poll的应用场景。
一，相关操作函数

#include <sys/timerfd.h>

int timerfd_create(int clockid, int flags);

int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);

int timerfd_gettime(int fd, struct itimerspec *curr_value);
二，timerfd_create

int timerfd_create(int clockid, int flags);

它是用来创建一个定时器描述符timerfd

第一个参数：clockid指定时间类型，有两个值：

CLOCK_REALTIME :Systemwide realtime clock. 系统范围内的实时时钟

CLOCK_MONOTONIC:以固定的速率运行，从不进行调整和复位 ,它不受任何系统time-of-day时钟修改的影响

第二个参数：flags可以是0或者O_CLOEXEC/O_NONBLOCK。

返回值：timerfd（文件描述符）

## errno 的宏定义

~~~cpp
#ifndef _ASM_GENERIC_ERRNO_BASE_H
#define _ASM_GENERIC_ERRNO_BASE_H

#define	EINPROGRESS	115	/* Operation now in progress */
#define	EINTR		 4	/* Interrupted system call */
#define	EISCONN		106	/* Transport endpoint is already connected */
#define	EAGAIN		11	/* Try again */
#define	EADDRINUSE	98	/* Address already in use */
#define	EADDRNOTAVAIL	99	/* Cannot assign requested address */
#define	ECONNRESET	104	/* Connection reset by peer */
#define	EACCES		13	/* Permission denied */
#define	EPERM		 1	/* Operation not permitted */
#define	EAFNOSUPPORT	97	/* Address family not supported by protocol */
#define	EALREADY	114	/* Operation already in progress */
#define	EBADF		 9	/* Bad file number */
#define	EFAULT		14	/* Bad address */
#define	ENOTSOCK	88	/* Socket operation on non-socket */


#endif
~~~
