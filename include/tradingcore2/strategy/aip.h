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
  // virtual void onTimeStamp(bool issim, TimeStamp ts, int index) override;

  virtual void onBuy(bool issim, TimeStamp ts, Money money, Volume volume,
                     Money fee) override;

 public:
  // void init(const char* assetsName, TimeType type, int timeParam, Money
  // money) {
  //   m_type = type;
  //   m_timeParam = timeParam;
  //   m_money = money;
  //   m_volume = 0;
  //   m_assetsName = assetsName;
  // }

 protected:
  // std::string m_assetsName;

  // TimeType m_type;
  // int m_timeParam;
  // Money m_money;
  // Volume m_volume;
};

CR2END

#endif  // __TRADINGCORE2_STRATEGY_AIP_H__