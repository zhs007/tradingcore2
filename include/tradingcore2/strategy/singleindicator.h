#ifndef __TRADINGCORE2_STRATEGY_SINGLEINDICATOR_H__
#define __TRADINGCORE2_STRATEGY_SINGLEINDICATOR_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/strategy.h>

#include <vector>

CR2BEGIN

class SingleIndicator : public Strategy {
 public:
  SingleIndicator(Wallet& wallet, Exchange& exchange)
      : Strategy(wallet, exchange) {}
  virtual ~SingleIndicator() {}

 public:
  virtual void onTimeStamp(TimeStamp ts);

 protected:
};

CR2END

#endif  // __TRADINGCORE2_STRATEGY_SINGLEINDICATOR_H__