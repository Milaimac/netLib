#pragma  once
#include <string>
#include <boost/operators.hpp>
#include "copyable.h"

namespace netLib
{
class Timestamp:public netLib::copyable,
                boost::equality_comparable<Timestamp>, 
                boost::less_than_comparable<Timestamp>{
public:
    // construct a trival Timestamp
    Timestamp()
    :microSecondSinceEpoch_(0){

    }
    // construct a Timestamp with a specific time
    explicit Timestamp(int64_t microSecondSinceEpochArg)
    :microSecondSinceEpoch_(microSecondSinceEpochArg){

    }
    // Timestamp support the copy,assign,dtor
    void swap(Timestamp& rhs){
        std::swap(this->microSecondSinceEpoch_, rhs.microSecondSinceEpoch_);
    }
    
    std::string toString() const;
    std::string toFormattedString(bool showMicroSeconds = true) const;

    bool valid() const {microSecondSinceEpoch_ > 0;}
    // microsecond 微秒，10^（-6）秒
    int64_t microSecondsSinceEpoch(){return  microSecondSinceEpoch_;}
    time_t secondsSinceEpoch(){return static_cast<time_t>(microSecondSinceEpoch_/kMicroSecondsPerSecond);}    //typedef long time_t 

    static Timestamp now();
    static Timestamp invalid(){
        return Timestamp();
    }

    static Timestamp fromUnixTime(time_t t){
        return  fromUnixTime(t, 0);
    }
    static Timestamp fromUnixTime(time_t t, int microseconds){
        return Timestamp(static_cast<int64_t>(t)*kMicroSecondsPerSecond + microseconds);
    }


    static const int kMicroSecondsPerSecond = 1000*1000;
private:
    int64_t microSecondSinceEpoch_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs){
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs){
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

// caculate the  diff
inline double timeDifference(Timestamp high, Timestamp low){
    int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
    return  static_cast<double>(diff / Timestamp::kMicroSecondsPerSecond);
}

inline Timestamp add(Timestamp timestamp, int seconds){
    int64_t delta = timestamp.microSecondsSinceEpoch() + seconds * Timestamp::kMicroSecondsPerSecond;
    return Timestamp(delta);
}
} // namespace netLib
