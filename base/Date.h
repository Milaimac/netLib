#pragma once
#include "Types.h"
#include "copyable.h"

struct tm;

namespace netLib{

class Date:public copyable{
public:
    struct YearMonthDay{
        int year;
        int month;
        int day;
    };

    static const int kDaysPerWeek = 7;
    static const int kJulianDayOf1970_01_01;

    // contrust a invalid Date
    Date()
    :JulianDaynumber_(0){

    }
    //  construct yyyy-mm-dd Date
    Date(int year, int mount, int day);
    
    explicit Date(int julianDaynum):JulianDaynumber_(julianDaynum){

    }

    explicit Date(const struct tm&);

    void swap(Date& that){
        std::swap(JulianDaynumber_, that.JulianDaynumber_);
    }

    bool valid() const{
        return JulianDaynumber_ > 0;
    }

    std::string toIsoString() const;

    struct YearMonthDay yearMonthDay() const;

    int year() const{
        return  yearMonthDay().year;
    }
    int mouth() const{
        return yearMonthDay().month;
    }
    int day() const{
        return yearMonthDay().day;
    }

    int weekDay() const{
        return (JulianDaynumber_ + 1)%kDaysPerWeek;
    }

    int julianDayNumber() const{
        return JulianDaynumber_;
    }

private:
    int JulianDaynumber_;

};

inline bool operator<(Date x, Date y){
    return x.julianDayNumber() < y.julianDayNumber();
}

inline bool operator==(Date x, Date y){
    return  x.julianDayNumber() == y.julianDayNumber();
}



}   // namespace netLib