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

void StrategySI::onTimeStamp(bool issim, TimeStamp ts, int index) {
  assert(this->m_pIndicator != NULL);

  auto cv = this->m_pIndicator->getSingleValue(index);
  assert(cv);

  CandleData cd;
  this->m_exchange.getData(this->m_assetsName.c_str(), index, cd);

  if (this->m_volume == 0) {
    if (cv->value >= this->m_minValBuy && cv->value < this->m_maxValBuy) {
      this->m_volume = this->m_wallet.buyAssets(this->m_assetsName.c_str(),
                                                this->m_money, ts);

      this->onTrading();

      this->m_buyPrice = cd.close;

      this->setStopLossPrice((1 - this->m_stoploss) * cd.close);
    }
  } else {
    if (cv->value >= this->m_minValSell && cv->value < this->m_maxValSell) {
      this->m_money = this->m_wallet.sellAssets(this->m_assetsName.c_str(),
                                                this->m_volume, ts);

      if (cd.close <= this->m_buyPrice) {
        this->onTradingFail();
      }

      this->m_volume = 0;
    } else {
      auto curmoney = this->onProcStopLoss(this->m_assetsName.c_str(), cd.close,
                                           this->m_volume, ts, index);

      if (curmoney > 0) {
        this->m_money = curmoney;
        this->m_volume = 0;
      }
    }
  }
}

void StrategySI::release() {
  if (this->m_pIndicator != NULL) {
    delete this->m_pIndicator;

    this->m_pIndicator = NULL;
  }
}

// void StrategySI::onBuy(bool issim, TimeStamp ts, int index,
//                        const tradingpb::Asset* pAsset, Money money) {
//   // this->m_wallet.deposit(this->m_money, ts);
//   // this->m_volume +=
//   //     this->m_wallet.buyAssets(this->m_assetsName.c_str(), this->m_money,
//   //     ts);
// }

CR2END