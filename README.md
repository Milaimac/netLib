<!--
 * @Date: 2021-12-07 11:48:30
 * @LastEditors: kafier
 * @LastEditTime: 2021-12-18 14:47:48
-->
# 一个高性能的C++网络库 NetLib

## 环境
Ubuntu18.04

g++ 7.5.0

## 项目介绍
NetLib是一个仿照muduo设计思想实现的一个http服务器， 整体的项目的IO模型是Reactor模型（non-blocking+IO multiplexing）。

## 技术亮点

- 定时器： 基于时间堆实现的定时器来关闭超时请求
- 异步日志库： 利用双缓冲技术实现的异步缓冲
- http服务器： 使用有限状态机对http请求报文进行解析， 并对其进行响应。
- 使用智能指针等RAII机制， 来减少内存泄漏的可能
- 实现优雅关闭连接
- 实现在高并发环境下， 在IO线程之间的任务调配

## 项目测试

使用webbench1.5给服务器做并发测试

## 项目核心模块

[异步日志库的功能和实现](https://github.com/Milaimac/netLib/blob/master/conclusion/%E5%A4%9A%E7%BA%BF%E7%A8%8B%E5%BC%82%E6%AD%A5%E6%97%A5%E5%BF%97%E5%BA%93.md)

[定时器模块的设计逻辑](https://github.com/Milaimac/netLib/blob/master/conclusion/%E5%AE%9A%E6%97%B6%E5%99%A8.md)

[IO模型]()

## 未来改进方向

- 将http服务器和网络库进行解耦， 让网络库能够适用于更多场景
- 尝试加入协程库

## 网络库性能测试



### 测试环境

os: ubuntu18.04 (wsl)

cpu: i5-8300H 2.30Ghz

内存大小: 4G


### 测试准备

\- 为了防止被实际的网络状态导致的带宽限制， 使用本地环境作为理想的测试环境

\- 使用webbnech， 开启1000个客户端， 测试事件为20s

\- 关闭所有输出和log

\- 线程池启用4线程

\- 为避免发送的延迟， 关闭nagle算法



### 测试结果

使用了日志功能的测试结果

![image-20211214153732550](https://gitee.com/systemcaller/bed/raw/master/img/20211214153733.png)

将日志功能关闭的测试结果

![image-20211214152557069](https://gitee.com/systemcaller/bed/raw/master/img/20211214152910.png)

没有使用异步日志的测试结果

![image-20211214160240974](https://gitee.com/systemcaller/bed/raw/master/img/20211214160242.png)

