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

 public:
  //   virtual void onBuy(bool issim, TimeStamp ts, Money money, Volume volume,
  //                      Money fee) override;

 public:
 protected:
};

CR2END

#endif  // __TRADINGCORE2_STRATEGY_NORMAL_H__