#ifndef __TRADINGCORE2_STRATEGY_NORMAL_H__
#define __TRADINGCORE2_STRATEGY_NORMAL_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/strategy.h>

#include <vector>

CR2BEGIN

class StrategyNormal final : public Strategy {
 public:
  static void regStrategy();

 protected:
  StrategyNormal(Wallet& wallet, Exchange& exchange)
      : Strategy(wallet, exchange) {}
  virtual ~StrategyNormal() {}

};

CR2END

#endif  // __TRADINGCORE2_STRATEGY_NORMAL_H__