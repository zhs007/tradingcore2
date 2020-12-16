#include <tradingcore2/exchange.h>
#include <tradingcore2/indicatormgr.h>
#include <tradingcore2/strategy/singleindicator2.h>
#include <tradingcore2/wallet.h>

CR2BEGIN

void StrategySI2::init(const char* assetsName, const char* nameIndicator,
                       int avgtimes, StrategySI2::FuncOnTimeStamp funcBuy,
                       StrategySI2::FuncOnTimeStamp funcSell, Money money) {
  assert(assetsName != NULL);
  assert(nameIndicator != NULL);
  assert(avgtimes >= 0);
  assert(money > 0);
  assert(funcBuy != NULL);
  assert(funcSell != NULL);

  this->m_assetsName = assetsName;

  auto mgr = IndicatorMgr::getSingleton();
  this->m_pIndicator = mgr->newIndicator(nameIndicator, avgtimes);

  this->m_pIndicator->build(
      this->m_exchange, this->m_assetsName.c_str(), 0,
      this->m_exchange.getDataLength(this->m_assetsName.c_str()));

  m_funcBuy = funcBuy;
  m_funcSell = funcSell;

  m_money = money;
  m_volume = 0;
}

void StrategySI2::onTimeStamp(bool issim, TimeStamp ts, int index) {
  assert(this->m_pIndicator != NULL);
  assert(this->m_funcBuy != NULL);
  assert(this->m_funcSell != NULL);

  auto cv = this->m_pIndicator->getSingleValue(index);
  assert(cv);

  CandleData cd;
  this->m_exchange.getData(this->m_assetsName.c_str(), index, cd);

  if (this->m_volume == 0) {
    if (this->m_funcBuy(&this->m_exchange, this->m_pIndicator, ts, index)) {
      this->m_volume = this->m_wallet.buyAssets(this->m_assetsName.c_str(),
                                                this->m_money, ts, 0, 0);

      this->onTrading();

      this->m_buyPrice = cd.close;

      this->setStopLossPrice((1 - this->m_stoploss) * cd.close);
    }
  } else {
    if (this->m_funcSell(&this->m_exchange, this->m_pIndicator, ts, index)) {
      this->m_money = this->m_wallet.sellAssets(this->m_assetsName.c_str(),
                                                this->m_volume, ts, 0, 0);

      if (cd.close <= this->m_buyPrice) {
        this->onTradingFail();
      }

      this->m_volume = 0;
    } else {
      auto curmoney = this->onProcStopLoss(this->m_assetsName.c_str(), cd.close,
                                           this->m_volume, ts, index, 0, 0);

      if (curmoney > 0) {
        this->m_money = curmoney;
        this->m_volume = 0;
      }
    }
  }
}

void StrategySI2::release() {
  if (this->m_pIndicator != NULL) {
    delete this->m_pIndicator;

    this->m_pIndicator = NULL;
  }
}

// void StrategySI2::onBuy(bool issim, TimeStamp ts, int index,
//                         const tradingpb::Asset* pAsset, Money money) {
//   // this->m_wallet.deposit(this->m_money, ts);
//   // this->m_volume +=
//   //     this->m_wallet.buyAssets(this->m_assetsName.c_str(), this->m_money,
//   //     ts);
// }

CR2END