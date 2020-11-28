#ifndef __TRADINGCORE2_STRATEGY_AIP_H__
#define __TRADINGCORE2_STRATEGY_AIP_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/strategy.h>

#include <vector>

CR2BEGIN

// StrategyAIP - automatic investment plan
class StrategyAIP final : public Strategy {
 public:
  enum TimeType {
    TT_NONE = 0,
    TT_WEEK = 1,
    TT_MONTH = 2,
  };

 public:
  static void regStrategy();

 protected:
  StrategyAIP(Wallet& wallet, Exchange& exchange)
      : Strategy(wallet, exchange) {}
  virtual ~StrategyAIP() {}

 public:
  virtual void onBuy(bool issim, TimeStamp ts, Money money, Volume volume,
                     Money fee) override;

};

CR2END

#endif  // __TRADINGCORE2_STRATEGY_AIP_H__