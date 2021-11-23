#pragma once
#include "copyable.h"
#include <time.h>
#include <memory>


namespace  netLib{

class TimeZone:public copyable{
public:
    explicit TimeZone(const char* zonefile);
    TimeZone (int eastOfutc, const char* tzname);
    TimeZone() = default;

    bool valid() const{
        return static_cast<bool>(data_);
    }
    struct tm tolocaltime(time_t secondSinceEpoch) const;
    time_t fromlocaltime(const struct tm&) const;
    // gmtime
    static struct tm toUtcTime(time_t secondSinceEpoch, bool yday = false);
    //timegm
    static time_t fromUtcTime(const struct tm&);
    // year [1900 - 2500], mouth [1 12], day [1 31]
    static time_t fromUtcTime(int year, int mouth, int day, 
                                int hour, int minute, int seconds);


    struct Data;
private:
    std::shared_ptr<Data> data_;
};
}