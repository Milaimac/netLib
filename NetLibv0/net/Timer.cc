#include "../net/Timer.h"
using namespace netLib;
using namespace netLib::net;

AtomicInt64 Timer::s_numCreated_;

void Timer::restart(Timestamp now){
    if(repeat_){
        expiration_ = addTime(now, interval_);
    }
    else{// return an invalid Timerstamp
        expiration_ = Timestamp::invalid();
    }
}

