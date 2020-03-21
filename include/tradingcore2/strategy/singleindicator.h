#ifndef __TRADINGCORE2_STRATEGY_SINGLEINDICATOR_H__
#define __TRADINGCORE2_STRATEGY_SINGLEINDICATOR_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/strategy.h>

#include <vector>

CR2BEGIN

class StrategySI : public Strategy {
 public:
  StrategySI(Wallet& wallet, Exchange& exchange)
      : Strategy(wallet, exchange),
        m_pIndicator(NULL),
        m_minValBuy(0),
        m_maxValBuy(0),
        m_minValSell(0),
        m_maxValSell(0),
        m_volume(0) {}
  virtual ~StrategySI() {}

 public:
  virtual void onTimeStamp(TimeStamp ts, int index);

 public:
  void initIndicator(const char* nameIndicator, int avgtimes);

  void init(IndicatorDataValue minvalbuy, IndicatorDataValue maxvalbuy,
            IndicatorDataValue minvalsell, IndicatorDataValue maxvalsell,
            Money money) {
    m_minValBuy = minvalbuy;
    m_maxValBuy = maxvalbuy;
    m_minValSell = minvalsell;
    m_maxValSell = maxvalsell;

    m_money = money;
    m_volume = 0;
  }

 protected:
  std::string m_assetsName;
  Indicator* m_pIndicator;

  IndicatorDataValue m_minValBuy;
  IndicatorDataValue m_maxValBuy;
  IndicatorDataValue m_minValSell;
  IndicatorDataValue m_maxValSell;
  Money m_money;
  Volume m_volume;
};

CR2END

#endif  // __TRADINGCORE2_STRATEGY_SINGLEINDICATOR_H__