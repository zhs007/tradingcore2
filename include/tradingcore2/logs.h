#ifndef __TRADINGCORE2_LOG_H__
#define __TRADINGCORE2_LOG_H__

#include <glog/logging.h>
#include <tradingcore2/basedef.h>

CR2BEGIN

void initLogs(const char* name);

void releaseLogs();

CR2END

#endif  // __TRADINGCORE2_UTILS_H__