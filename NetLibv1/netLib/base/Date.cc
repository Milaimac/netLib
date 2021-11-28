#include "Date.h"
#include <stdio.h>

namespace netLib
{
namespace detail
{
char require_32_bit_integer_at_least[sizeof(int) >= sizeof(int32_t) ? 1: -1];

int getJulianDayNumber(int year,int mouth,int day){
    (void) require_32_bit_integer_at_least;
    // 这是公历转为儒略日的公式
    int a = (14-mouth)/12;
    int y = year + 4800 - a;
    int m = mouth + 12*a - 3;
    return day + (153*m + 2) / 5 + y*365 + y/4 - y/100 + y/400 - 32045;
}

// 通过这些公式从儒略日转换为公历？ 确实如此， 计算后仍然是getJulianDayNumber之前的数字
struct Date::YearMonthDay getYearMonthDay(int julianDayNumber)
{
    int a = julianDayNumber + 32044;
    int b = (4 * a + 3) / 146097;
    int c = a - ((b * 146097) / 4);
    int d = (4 * c + 3) / 1461;
    int e = c - ((1461 * d) / 4);
    int m = (5 * e + 2) / 153;
    Date::YearMonthDay ymd;
    ymd.day = e - ((153 * m + 2) / 5) + 1;
    ymd.month = m + 3 - 12 * (m / 10);
    ymd.year = b * 100 + d - 4800 + (m / 10);
    return ymd;
}

} // namespace detail
const int Date::kJulianDayOf1970_01_01 = detail::getJulianDayNumber(1970, 1, 1);
} // namespace netLib

using namespace netLib;
using namespace netLib::detail;

Date::Date(int y, int m, int d)
  : JulianDaynumber_(getJulianDayNumber(y, m, d))
{
}

Date::Date(const struct tm& t)
  : JulianDaynumber_(getJulianDayNumber(
        t.tm_year+1900,
        t.tm_mon+1,
        t.tm_mday))
{
}

string Date::toIsoString() const
{
  char buf[32];
  YearMonthDay ymd(yearMonthDay());
  snprintf(buf, sizeof buf, "%4d-%02d-%02d", ymd.year, ymd.month, ymd.day);
  return buf;
}

Date::YearMonthDay Date::yearMonthDay() const
{
  return getYearMonthDay(JulianDaynumber_);
}

