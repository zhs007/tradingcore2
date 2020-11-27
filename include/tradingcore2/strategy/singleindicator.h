#ifndef __TRADINGCORE2_STRATEGY_SINGLEINDICATOR_H__
#define __TRADINGCORE2_STRATEGY_SINGLEINDICATOR_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/strategy.h>

#include <vector>

CR2BEGIN

class StrategySI final : public Strategy {
 public:
  StrategySI(Wallet& wallet, Exchange& exchange)
      : Strategy(wallet, exchange),
        m_pIndicator(NULL),
        m_minValBuy(0),
        m_maxValBuy(0),
        m_minValSell(0),
        m_maxValSell(0),
        m_volume(0),
        m_buyPrice(0) {}
  virtual ~StrategySI() { this->release(); }

 public:
  virtual void onTimeStamp(bool issim, TimeStamp ts, int index) override;

  // virtual void onBuy(bool issim, TimeStamp ts, Money money, Volume volume,
  //                    Money fee) override;

 public:
  void init(const char* assetsName, const char* nameIndicator, int avgtimes,
            IndicatorDataValue minvalbuy, IndicatorDataValue maxvalbuy,
            IndicatorDataValue minvalsell, IndicatorDataValue maxvalsell,
            Money money);

  void release();

 protected:
  std::string m_assetsName;
  Indicator* m_pIndicator;

  IndicatorDataValue m_minValBuy;
  IndicatorDataValue m_maxValBuy;
  IndicatorDataValue m_minValSell;
  IndicatorDataValue m_maxValSell;
  Money m_money;
  Volume m_volume;

  Money m_buyPrice;
};

CR2END

#endif  // __TRADINGCORE2_STRATEGY_SINGLEINDICATOR_H__