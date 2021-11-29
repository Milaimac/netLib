#include "TimeZone.h"
#include "noncopyable.h"
#include "Types.h"
#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>
#include "Date.h"
namespace  netLib{
namespace detail
{
    

struct Transition{

    time_t gmttime;
    time_t localtime;
    int localtimeIdx;

    Transition(time_t t, time_t l, int localIdx):gmttime(t), localtime(l), localtimeIdx(localIdx){

    }
};

struct Comp{
    bool compareGmt;

    Comp(bool gmt):compareGmt(gmt){

    }

    bool operator()(const Transition& lhs, const Transition& rhs)const{
        // compareGmt为true，比较gmttime， 否则比较localtime
        if(compareGmt){
            return lhs.gmttime < rhs.gmttime;
        }else{
            return lhs.localtime < rhs.localtime;
        }
    }

    bool equal (const Transition& lhs, const Transition& rhs){
        if(compareGmt){
            return lhs.gmttime == rhs.gmttime;
        }
        else{
            return lhs.localtime == rhs.localtime;
        }
    }
};

struct Localtime{
    time_t gmtOffset;
    bool isDst;
    int arrbIdx;
    Localtime(time_t offset, bool dst, int arrb):gmtOffset(offset), isDst(dst),arrbIdx(arrb){

    }
};

inline void fillHMS(unsigned seconds,   struct tm* utc){
    utc->tm_sec = seconds % 60;
    unsigned minutes = seconds / 60;
    utc->tm_min = minutes % 60;
    utc->tm_hour = minutes/60;
}

} // namespace detail

const int kSecondsPerday = 24*60*60;
}

using namespace netLib;
using namespace detail;
using namespace std;

struct TimeZone::Data{
    vector<detail::Transition> transitions;
    vector<detail::Localtime> localtimes;
    vector<string> names;
    string abbreviation;
};
namespace netLib{
namespace detail
{
class File:noncopyable{
public:
    File(const char* file)
    :fp_(::fopen(file,"rb")){

    }
    ~File(){
        if(fp_){
            ::fclose(fp_);
        }
    }

    string readBytes(int n){
        char buf[n];
        ssize_t nr = ::fread(buf, 1, n, fp_);
        if(nr != n){
            throw  logic_error("no enough data");
        }
        return string(buf, n);
    }

    int32_t readInt32(){
        int32_t x = 0;
        ssize_t nr = ::fread(&x, 1, sizeof(int32_t), fp_);
        if(nr != sizeof(int32_t)){
            throw logic_error("bad int32_t data");
        }
        return be32toh(x);
    }

    uint8_t readUInt8(){
        uint8_t x = 0;
        ssize_t nr = ::fread(&x, 1, sizeof(uint8_t), fp_);
        if(nr != sizeof(uint8_t)){
            throw logic_error("bad uint8_t data");
        }
        return x;
    }
    bool valid() const{return fp_;}
private:
    FILE* fp_;
};

bool readTimeZoneFile(const char* zonefile, struct TimeZone::Data* data){
    File f(zonefile);
    if(f.valid()){
        try{
            string head = f.readBytes(4);
            if(head != "TZif"){
                throw logic_error("bad head");
            }
            string version = f.readBytes(1);
            f.readBytes(15);
            // gmt  是格林威志时间， 也就是ust = 0的时区
            int32_t  isgmtcnt = f.readInt32();
            int32_t isstdcnt = f.readInt32();
            int32_t leapcnt = f.readInt32();
            int32_t timecnt = f.readInt32();
            int32_t typecnt = f.readInt32();
            int32_t charcnt = f.readInt32();


            vector<int32_t> trans;
            vector<int> localtimes;
            trans.reserve(timecnt);
            for(int i = 0;i < timecnt;++i){
                trans.push_back(f.readInt32());
            }

            for(int i = 0;i < timecnt;++i){
                uint8_t local = f.readUInt8();
                localtimes.push_back(local);
            }

            for(int i = 0;i < typecnt;++i){
                int32_t gmtoff = f.readInt32();
                uint8_t isdst = f.readUInt8();
                uint8_t abbrind = f.readUInt8();

                data->localtimes.push_back(Localtime(gmtoff, isdst, abbrind));
            }

            for(int i = 0;i < timecnt;++i){
                int localIdx =localtimes[i];
                time_t localtime = trans[i] + data->localtimes[localIdx].gmtOffset;
                data->transitions.push_back(Transition(trans[i], localtime, localIdx));
            }

            data->abbreviation = f.readBytes(charcnt);

            // leapcnt
            for (int i = 0; i < leapcnt; ++i)
            {
                // int32_t leaptime = f.readInt32();
                // int32_t cumleap = f.readInt32();
            }
            (void) isstdcnt;
            (void) isgmtcnt;
        }
        catch(logic_error& e){
            fprintf(stderr, "%s\n", e.what());
        }
    }
    return  true;
}

// sentry是哨兵的意思
const Localtime* findLocaltime(const TimeZone::Data& data, Transition sentry, Comp comp){
    const Localtime* local = NULL;
    // 如果说data.transition为空，并且sentry小于data.transition.front()的gmttime和localtime， 那么local 就等于现在这个data
    if(data.transitions.empty() || comp(sentry, data.transitions.front())){
        local = &data.localtimes.front();
    }
    else{
        // lower_bound返回第一个大于等于sentry的数字，
        vector<Transition>::const_iterator transI = lower_bound(data.transitions.begin(), 
                                                                data.transitions.end(), 
                                                                sentry, 
                                                                comp);
        // 根据localtimeIdx在data_的localtimes找到独赢的Localtime对象
        if(transI != data.transitions.end()){
            if(!comp.equal(sentry, *transI)){// 如果不相等，那么--tranSI， 
                assert(transI != data.transitions.begin());
                --transI;
            }
            local = &data.localtimes[transI->localtimeIdx];
        }
        else{
            local = &data.localtimes[data.transitions.back().localtimeIdx];
        }
    }
    return local;
}
} // namespace detail
}// namespace netLib

TimeZone::TimeZone(const char *zonefile)
:data_(new TimeZone::Data)
{   
    if(!detail::readTimeZoneFile(zonefile, data_.get())){
        data_.reset();
    }
}

TimeZone::TimeZone(int eastOfUtc, const char* name)
:data_(new TimeZone::Data){
    data_->localtimes.push_back(detail::Localtime(eastOfUtc, false,0 ));
    data_->abbreviation = name;
}

struct tm TimeZone::tolocaltime(time_t seconds) const{
    struct tm localtime;
    memZero(&localtime, sizeof(localtime));
    assert(data_ != NULL);
    const Data& data(*data_);
    //得到的seconds，是从日历时间，变为本地时间
    detail::Transition sentry(seconds, 0, 0);
    const detail::Localtime* local = findLocaltime(data, sentry, detail::Comp(true)); //true means it will compare the gmt time
    // tm_isdst 是判断是否为夏令时（daylight Saving time）dst，夏令时就是把钟调快一小时，只不过中国早就不使用了
    // tm_gmtoff应该代表的是当前时区相对UTC往东的时间差（即东为正西为负），与timezone变量相反。
    // tm_zone 是时区的名称
    if(local){
        time_t localSeconds = local->gmtOffset + seconds;            // 用本地时区的时间偏置得到本地时间
        ::gmtime_r(&localSeconds, &localtime);
        localtime.tm_isdst = local->isDst;
        localtime.tm_gmtoff = local->gmtOffset;
        localtime.tm_zone  = &data.abbreviation[local->arrbIdx];        
    }
    return localtime;
}

time_t TimeZone::fromlocaltime(const struct tm& localTm) const{
    assert(data_ != NULL);
    const TimeZone::Data data(*data_);
    
    struct tm tmp = localTm;
    time_t seconds = timegm(&tmp);          //timegm不考虑本地时间设置，所以后续操作需要考虑它为本地时间
    detail::Transition sentry(0, seconds, 0);       
    const detail::Localtime* local = findLocaltime(data, sentry, detail::Comp(false));
    if(localTm.tm_isdst){
        struct tm tryTm = tolocaltime(seconds - local->gmtOffset);
        if(!tryTm.tm_isdst
            && tryTm.tm_hour == localTm.tm_hour
            && tryTm.tm_min == localTm.tm_min){
            seconds -= 3600; // 夏令时减少一个小时
        }
    }
    return seconds - local->gmtOffset;
}
// 从secondsSinceEpoch 得到 days + seconds -- 通过fillHMS得到这一天的hour、minutes、seconds -- 构造Date对象 Date(days + Date::kJulianDayOf...)
// 通过date对象的转换函数 yearMonthDay()得到 ymd，

// 事实上，这个搞了这么复杂，最终得到和原始数据没什么区别
struct tm TimeZone::toUtcTime(time_t secondsSinceEpoch, bool yday){
    // 这个secondsSinceEpoch应该是从1970年开始的时间
    struct tm utc;
    memZero(&utc, sizeof(utc));
    utc.tm_zone = "GMT";
    int seconds = static_cast<int> (secondsSinceEpoch % kSecondsPerday);
    int days = static_cast<int> (secondsSinceEpoch / kSecondsPerday);
    if(seconds < 0){
        seconds += kSecondsPerday;
        --days;
    }
    detail::fillHMS(seconds, &utc);
    Date date(days + Date::kJulianDayOf1970_01_01);
    Date::YearMonthDay ymd = date.yearMonthDay();
    utc.tm_year = ymd.year - 1900;
    utc.tm_mon = ymd.month - 1;
    utc.tm_mday = ymd.day;
    utc.tm_wday = date.weekDay();   

    if(yday){
        Date startOfYear(ymd.year, 1, 1);
        // tm.tm_yday 指的是一年中，从1月1日到现在有多少天
        utc.tm_yday = date.julianDayNumber() - startOfYear.julianDayNumber();
    }

    return utc;
}
// 从utc构造时间
// utc -- julianDay -- seconds?
time_t TimeZone::fromUtcTime(const struct tm& utc){
    return fromUtcTime(utc.tm_year + 1900, utc.tm_mon+1, utc.tm_mday+1, utc.tm_hour, utc.tm_min, utc.tm_sec);
}

time_t TimeZone::fromUtcTime(int year, int month,int day, int hour,int minute, int second){
    Date date(year, month, day);
    int secondsInday = hour * 3600 + minute * 60 + second;
    time_t  days = date.julianDayNumber() - Date::kJulianDayOf1970_01_01;
    return  days*kSecondsPerday + secondsInday;
}

// to 就是转化为time_t 
// from 就是转化为seconds