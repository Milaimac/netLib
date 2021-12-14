<!--
 * @Date: 2021-12-07 11:48:30
 * @LastEditors: kafier
 * @LastEditTime: 2021-12-09 22:22:18
-->
# 一个高性能的C++网络库 NetLib

## 环境
Ubuntu18.04

g++ 7.5.0

## 项目介绍
NetLib是一个仿照muduo设计思想实现的一个网络库， 整体的项目的IO模型是Reactor模型（non-blocking+IO multiplexing）。

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

[]
