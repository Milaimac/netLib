#pragma once

#include "../base/copyable.h"
#include <stdint.h>
namespace  netLib{
namespace net
{
class Timer;
//
// An opaque identifier, for canceling Timer
// opaque -- 不透明

class TimerId:public netLib::copyable{
public:
    TimerId()
    :timer_(nullptr),
    sequence_(0)
    {}
    TimerId(Timer* timer, int64_t seq)
    :timer_(timer),
    sequence_(seq)
    {}

    friend class TimerQueue;
private:
    Timer* timer_;
    int64_t sequence_;
};

} // namespace net
    
} // namespace netLib

// the Timer's sequence