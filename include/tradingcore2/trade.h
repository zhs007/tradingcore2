#ifndef __TRADINGCORE2_TRADE_H__
#define __TRADINGCORE2_TRADE_H__

#include <tradingcore2/basedef.h>
#include <string>

CR2BEGIN

enum TradeType { TT_BUY = 0, TT_SELL = 1 };

struct Trade {
  TradeType tradeType;
  std::string assetsName;
  Money money;
  Volume volume;
  TimeStamp ts;
};

CR2END

#endif  // __TRADINGCORE2_TRADE_H__