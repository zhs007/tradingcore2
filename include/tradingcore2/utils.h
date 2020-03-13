#ifndef __TRADINGCORE2_UTILS_H__
#define __TRADINGCORE2_UTILS_H__

#include <tradingcore2/basedef.h>
#include <ctime>

CR2BEGIN

TimeStamp str2timestampUTC(const char* str, const char* format);

void str2time(const char* str, const char* format, tm& t);

TimeStamp time2timestampUTC(tm& t);

CR2END

#endif  // __TRADINGCORE2_UTILS_H__