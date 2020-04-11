#ifndef __TRADINGCORE2_LOG_H__
#define __TRADINGCORE2_LOG_H__

#include <glog/logging.h>
#include <tradingcore2/basedef.h>

CR2BEGIN

class LogHelper {
 public:
  LogHelper(const char* name);
  ~LogHelper();
};

CR2END

#endif  // __TRADINGCORE2_UTILS_H__