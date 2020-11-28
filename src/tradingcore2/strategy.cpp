
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/strategy.h>
#include <tradingcore2/utils.h>
#include <tradingcore2/wallet.h>

#include <functional>

CR2BEGIN

void Strategy::simulateTrading() {
  this->m_pCCData =
      CtrlConditionMgr::getSingleton()->newCtrlConditionData(*this);

  auto f = std::bind(&Strategy::onSimulateTradingTimeStamp, this,
                     std::placeholders::_2, std::placeholders::_3);

  this->m_exchange.forEachTimeStamp(f, 0, 0);

  CtrlConditionMgr::getSingleton()->deleteCtrlConditionData(*this,
                                                            this->m_pCCData);

  this->m_pCCData = NULL;
}

void Strategy::onSimulateTradingTimeStamp(TimeStamp ts, int index) {
  this->onTimeStamp(true, ts, index);
}

Money Strategy::onProcStopLoss(const char* assetsName, Money curPrice,
                               Volume volume, TimeStamp ts, int index) {
  if (this->m_curStopLossPrice > 0) {
    if (curPrice <= this->m_curStopLossPrice) {
      m_stoplossNums++;

      auto money = this->m_wallet.sellAssets(assetsName, volume, ts);

      return money;
    }
  }

  return 0;
}

void Strategy::print() {
  printf("win rate: %.3f%%\n",
         (1 - (float)this->m_stoplossNums / this->m_tradingNums) * 100);
}

void Strategy::getTrainResult(TrainResult& tr) {
  tr.tradingNums = this->m_tradingNums;
  tr.stoplossNums = this->m_stoplossNums;
  tr.winRate = 1 - (float)this->m_stoplossNums / this->m_tradingNums;
}

void Strategy::onTimeStamp(bool issim, TimeStamp ts, int index) {
  CtrlConditionMgr::getSingleton()->procStrategy(*this, this->m_pCCData, issim,
                                                 ts, index);
}

void Strategy::buy(bool issim, TimeStamp ts) {
  auto buy = this->m_strategy.paramsbuy();

  if (buy.initmoney() > 0 && buy.permoney() > 0) {
    auto m = buy.initmoney() * buy.permoney();

    Volume volume = ZEROVOLUME;
    Money price = ZEROMONEY;
    Money fee = ZEROMONEY;

    bool isok = m_exchange.calculateVolume(
        this->m_strategy.asset().code().c_str(), ts, m, volume, price, fee);
    assert(isok);
    assert(price > ZEROMONEY);

    this->onBuy(issim, ts, m, volume, fee);

    this->m_wallet.buyAssets(this->m_strategy.asset().code().c_str(), m, ts);
  } else if (buy.volume() > 0) {
  } else if (buy.aipmoney() > 0) {
    auto m = buy.aipmoney();

    Volume volume = ZEROVOLUME;
    Money price = ZEROMONEY;
    Money fee = ZEROMONEY;

    bool isok = m_exchange.calculateVolume(
        this->m_strategy.asset().code().c_str(), ts, m, volume, price, fee);
    assert(isok);
    assert(price > ZEROMONEY);

    this->onBuy(issim, ts, m, volume, fee);

    this->m_wallet.buyAssets(this->m_strategy.asset().code().c_str(), m, ts);
  }
}

void Strategy::onBuy(bool issim, TimeStamp ts, Money money, Volume volume,
                     Money fee) {
  assert(money <= this->m_handMoney);

  this->m_handMoney -= money;
  this->m_costMoney += money;
  this->m_fee += fee;
  this->m_volume += volume;
  this->m_price = this->m_costMoney / this->m_volume;
}

void Strategy::sell(bool issim, TimeStamp ts) {
  auto sell = this->m_strategy.paramssell();

  if (sell.volume() > 0) {
    auto v = sell.volume();
    if (v > this->m_volume) {
      v = this->m_volume;
    }

    if (v > 0) {
      auto m = this->m_wallet.sellAssets(
          this->m_strategy.asset().code().c_str(), v, ts);

      this->onSell(issim, ts, m, v, 0);
    }
  } else if (sell.pervolume() > 0) {
    auto v = sell.pervolume() * this->m_volume;

    if (v > 0) {
      auto m = this->m_wallet.sellAssets(
          this->m_strategy.asset().code().c_str(), v, ts);

      this->onSell(issim, ts, m, v, 0);
    }
  } else if (sell.money() > 0) {
    // auto m = buy.aipmoney();

    // Volume volume = ZEROVOLUME;
    // Money price = ZEROMONEY;
    // Money fee = ZEROMONEY;

    // bool isok = m_exchange.calculateVolume(
    //     this->m_strategy.asset().code().c_str(), ts, m, volume, price, fee);
    // assert(isok);
    // assert(price > ZEROMONEY);

    // this->onBuy(issim, ts, m, volume, fee);

    // this->m_wallet.buyAssets(this->m_strategy.asset().code().c_str(), m, ts);
  }
}

void Strategy::onSell(bool issim, TimeStamp ts, Money money, Volume volume,
                      Money fee) {
  assert(volume <= this->m_volume);

  this->m_handMoney += money;
  this->m_fee += fee;
  this->m_volume -= volume;

  if (this->m_volume > 0) {
    this->m_costMoney = this->m_price * this->m_volume;
  } else {
    this->m_costMoney = 0;
  }
}

CR2END