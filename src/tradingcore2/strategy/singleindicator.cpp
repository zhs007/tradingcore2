#include <tradingcore2/exchange.h>
#include <tradingcore2/indicatormgr.h>
#include <tradingcore2/strategy/singleindicator.h>
#include <tradingcore2/wallet.h>

CR2BEGIN

void StrategySI::init(const char* assetsName, const char* nameIndicator,
                      int avgtimes, IndicatorDataValue minvalbuy,
                      IndicatorDataValue maxvalbuy,
                      IndicatorDataValue minvalsell,
                      IndicatorDataValue maxvalsell, Money money) {
  this->m_assetsName = assetsName;

  auto mgr = IndicatorMgr::getSingleton();
  this->m_pIndicator = mgr->newIndicator(nameIndicator, avgtimes);

  this->m_pIndicator->build(
      this->m_exchange, this->m_assetsName.c_str(), 0,
      this->m_exchange.getDataLength(this->m_assetsName.c_str()));

  m_minValBuy = minvalbuy;
  m_maxValBuy = maxvalbuy;
  m_minValSell = minvalsell;
  m_maxValSell = maxvalsell;

  m_money = money;
  m_volume = 0;
}

void StrategySI::onTimeStamp(TimeStamp ts, int index) {
  assert(this->m_pIndicator != NULL);

  auto cv = this->m_pIndicator->getSingleValue(index);
  assert(cv);

  if (this->m_volume == 0) {
    if (cv->value >= this->m_minValBuy && cv->value < this->m_maxValBuy) {
      this->m_volume = this->m_wallet.buyAssets(this->m_assetsName.c_str(),
                                                this->m_money, ts);
    }
  } else {
    if (cv->value >= this->m_minValSell && cv->value < this->m_maxValSell) {
      this->m_money = this->m_wallet.sellAssets(this->m_assetsName.c_str(),
                                                this->m_volume, ts);
      this->m_volume = 0;
    }
  }
}

CR2END