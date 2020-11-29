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