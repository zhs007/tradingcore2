#ifndef __TRADINGCORE2_STRATEGY_BUYANDHOLD_H__
#define __TRADINGCORE2_STRATEGY_BUYANDHOLD_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/strategy.h>

#include <vector>

CR2BEGIN

class StrategyBAH final : public Strategy {
 public:
  StrategyBAH(Wallet& wallet, Exchange& exchange)
      : Strategy(wallet, exchange), m_money(0), m_volume(0) {}
  virtual ~StrategyBAH() {}

 public:
  virtual void onTimeStamp(TimeStamp ts, int index) override;

 public:
  void init(const char* assetsName, Money money) {
    m_money = money;
    m_volume = 0;
    m_assetsName = assetsName;
  }

 protected:
  std::string m_assetsName;

  Money m_money;
  Volume m_volume;
};

CR2END

#endif  // __TRADINGCORE2_STRATEGY_BUYANDHOLD_H__