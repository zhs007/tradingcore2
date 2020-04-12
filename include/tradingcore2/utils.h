#ifndef __TRADINGCORE2_UTILS_H__
#define __TRADINGCORE2_UTILS_H__

#include <tradingcore2/basedef.h>

#include <ctime>
#include <functional>
#include <string>

CR2BEGIN

TimeStamp str2timestampUTC(const char* str, const char* format);

void str2time(const char* str, const char* format, tm& t);

TimeStamp time2timestampUTC(tm& t);

// calcQuartile - sortedArr is a sorted array
void calcQuartile(float* sortedArr, size_t stride, size_t n, float& q1,
                  float& q2, float& q3);

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

// getVersion - get version from ./VERSION
const char* getVersion();

CR2END

#endif  // __TRADINGCORE2_UTILS_H__