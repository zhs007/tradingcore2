#ifndef __TRADINGCORE2_UTILS_H__
#define __TRADINGCORE2_UTILS_H__

#include <tradingcore2/basedef.h>

#include <ctime>
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
void joinPath(std::string& str, const char* root, const char* fn);

CR2END

#endif  // __TRADINGCORE2_UTILS_H__