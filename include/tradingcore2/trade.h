#ifndef __TRADINGCORE2_TRADE_H__
#define __TRADINGCORE2_TRADE_H__

#include <tradingcore2/basedef.h>
#include <string>

CR2BEGIN

struct Trade {
  std::string assetsName;
  Money money;
  Volume volume;
  TimeStamp ts;
};

CR2END

#endif  // __TRADINGCORE2_TRADE_H__