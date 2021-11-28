/*
 * @Date: 2021-11-27 21:48:47
 * @LastEditors: kafier
 * @LastEditTime: 2021-11-27 21:48:48
 */
#pragma once
#include <memory>
#include <vector>
#include "EventLoopThread.h"
#include "base/Logging.h"
#include "base/noncopyable.h"

class EventLoopThreadPool : noncopyable
{
public:
    EventLoopThreadPool(EventLoop *baseLoop, int numThreads);

    ~EventLoopThreadPool() { LOG << "~EventLoopThreadPool()"; }
    void start();

    EventLoop *getNextLoop();

private:
    EventLoop *baseLoop_;
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::shared_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop *> loops_;
};