#include "Timestamp.h"
#include <stdio.h>
#include <string>
#include <inttypes.h>
#include <sys/time.h>
using namespace netLib;

static_assert(sizeof(Timestamp) == sizeof(int64_t), "Timestamp should be same size as int64_t");

std::string Timestamp::toString() const{
    char buf[32] = {0};
    int64_t seconds = microSecondSinceEpoch_ / kMicroSecondsPerSecond;
    int64_t microseconds = microSecondSinceEpoch_ % kMicroSecondsPerSecond;
    snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
    return buf;
}

std::string Timestamp::toFormattedString(bool showMicroseconds)const{
    char buf[64] = {0};
    struct tm tm_time;
    time_t seconds = static_cast<time_t> (microSecondSinceEpoch_ / kMicroSecondsPerSecond);
    gmtime_r(&seconds, &tm_time);

    if(showMicroseconds){
        int microseconds = static_cast<int> (microSecondSinceEpoch_%kMicroSecondsPerSecond);
        snprintf(buf, sizeof(buf),"%4d%02d%02d %02d:%02d:%02d.%06d", 
        tm_time.tm_year + 1900 , tm_time.tm_mon+1, tm_time.tm_mday, tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, microseconds);
    }
    else{
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d", 
        tm_time.tm_year+1900, tm_time.tm_mon+1, tm_time.tm_mday, tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    }
    return buf;
}

Timestamp Timestamp::now(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}