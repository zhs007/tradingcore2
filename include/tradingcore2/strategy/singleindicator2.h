#ifndef __TRADINGCORE2_STRATEGY_SINGLEINDICATOR2_H__
#define __TRADINGCORE2_STRATEGY_SINGLEINDICATOR2_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/strategy.h>

#include <functional>
#include <vector>

CR2BEGIN

class StrategySI2 final : public Strategy {
 public:
  // FuncOnTimeStamp - for StrategySI2
  typedef std::function<bool(Exchange* pExchange, Indicator* pIndicator,
                             TimeStamp ts, int index)>
      FuncOnTimeStamp;

 public:
  StrategySI2(Wallet& wallet, Exchange& exchange)
      : Strategy(wallet, exchange),
        m_pIndicator(NULL),
        m_funcBuy(NULL),
        m_funcSell(NULL),
        m_volume(0),
        m_buyPrice(0) {}
  virtual ~StrategySI2() { this->release(); }

 public:
  virtual void onTimeStamp(bool issim, TimeStamp ts, int index) override;

  // virtual void onBuy(bool issim, TimeStamp ts, Money money, Volume volume,
  //                    Money fee) override;

 public:
  void init(const char* assetsName, const char* nameIndicator, int avgtimes,
            StrategySI2::FuncOnTimeStamp funcBuy,
            StrategySI2::FuncOnTimeStamp funcSell, Money money);

  void release();

 protected:
  std::string m_assetsName;
  Indicator* m_pIndicator;

  FuncOnTimeStamp m_funcBuy;
  FuncOnTimeStamp m_funcSell;
  Money m_money;
  Volume m_volume;

  Money m_buyPrice;
};

CR2END

#endif  // __TRADINGCORE2_STRATEGY_SINGLEINDICATOR2_H__