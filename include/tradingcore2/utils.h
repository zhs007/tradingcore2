#ifndef __TRADINGCORE2_UTILS_H__
#define __TRADINGCORE2_UTILS_H__

#include <google/protobuf/util/json_util.h>
#include <tradingcore2/basedef.h>

#include <ctime>
#include <functional>
#include <string>

CR2BEGIN

// -------------------------------------------------------------------------------------
// time

TimeStamp str2timestampUTC(const char* str, const char* format);

void str2time(const char* str, const char* format, tm& t);

TimeStamp time2timestampUTC(tm& t);

void timestamp2timeUTC(time_t ts, tm& t);

// getYearWeek - get week of the year with ts
int getYearWeek(time_t ts);

// getYearWeekEx - it's like 202001
int getYearWeekEx(time_t ts);

// getYearMonth - get month of the year with ts
int getYearMonth(time_t ts);

// getYearMonthEx - it's like 202001
int getYearMonthEx(time_t ts);

// getYear - get year
int getYear(time_t ts);

// getDate - get date, it's like 20201001
int getDate(time_t ts);

// calcWeekOffWithWeekDay -
// 主要用于定投，计算2个时间点之间的周差，以特定周几为基准
// 譬如以周三为基准，这周二相对上周五其实不足一周
// 譬如以周三为基准，这周三相对上周五是一周
int calcWeekOffWithWeekDay(time_t lastts, time_t ts, int weekday);

// calcMonthOffWithMonthDay -
// 主要用于定投，计算2个时间点之间的月差，以特定几号为基准
// 譬如以30号为基准，3月31号相对1月30号是2个月
int calcMonthOffWithMonthDay(time_t lastts, time_t ts, int monthday);

// getNextSunday - 返回下一个周日的0点0分0秒
time_t getNextSunday(time_t ts);

// getNextMonth - 返回下一个月1号的0点0分0秒
time_t getNextMonth(time_t ts);

// calcMonthOff - 计算月差，仅考虑月差，12月1号和11月31号，相差1个月
int calcMonthOff(time_t ts0, time_t ts1);

// -------------------------------------------------------------------------------------
// math

// calcQuartile - sortedArr is a sorted array
void calcQuartile(float* sortedArr, size_t stride, size_t n, float& q1,
                  float& q2, float& q3);

// -------------------------------------------------------------------------------------
// protobuf

void logProtobuf(const char* title, const google::protobuf::Message& message);

void saveProtobuf(const char* fn, const google::protobuf::Message& message);

// -------------------------------------------------------------------------------------
// file system

// joinPath - "/dir1", "f0.txt" => "/dir1/f0.txt"
//            "/dir1/", "f0.txt" => "/dir1/f0.txt"
std::string joinPath(const char* root, const char* fn);

// scaleValue - 1.57, 0.05 -> 1.60
//              -0.23, 0.05 -> -0.25
IndicatorDataValue scaleValue(IndicatorDataValue val, IndicatorDataValue off);

// FuncOnForEachFile - for foreachPath
typedef std::function<void(const char* dir, const char* fn)> FuncOnForEachFile;

// foreachPathWithExt - for each path with ext filename
void foreachPathWithExt(const char* dir, const char* extfn,
                        FuncOnForEachFile func);

// -------------------------------------------------------------------------------------
// version

// getVersion - get version from ./VERSION
const char* getVersion();

CR2END

#endif  // __TRADINGCORE2_UTILS_H__