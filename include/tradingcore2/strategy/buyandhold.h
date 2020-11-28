#ifndef __TRADINGCORE2_STRATEGY_BUYANDHOLD_H__
#define __TRADINGCORE2_STRATEGY_BUYANDHOLD_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/strategy.h>

#include <vector>

CR2BEGIN

class StrategyBAH final : public Strategy {
 public:
  static void regStrategy();

 protected:
  StrategyBAH(Wallet& wallet, Exchange& exchange)
      : Strategy(wallet, exchange) {}
  virtual ~StrategyBAH() {}

};

CR2END

#endif  // __TRADINGCORE2_STRATEGY_BUYANDHOLD_H__