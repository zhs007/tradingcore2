
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/strategy.h>
#include <tradingcore2/utils.h>
#include <tradingcore2/wallet.h>

#include <functional>

CR2BEGIN

void Strategy::simulateTrading() {
  auto f = std::bind(&Strategy::onSimulateTradingTimeStamp, this,
                     std::placeholders::_2, std::placeholders::_3);

  this->m_exchange.forEachTimeStamp(f, 0, 0);
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
  CtrlConditionMgr::getSingleton()->procStrategy(*this, issim, ts, index);
  // this->onCtrlConditionBuy(issim, ts, index);
}

// void Strategy::onCtrlConditionBuy(bool issim, TimeStamp ts, int index) {
//   auto asset = this->m_strategy.asset();

//   auto f = std::bind(&Strategy::onBuy, this, std::placeholders::_1,
//                      std::placeholders::_2, std::placeholders::_3,
//                      std::placeholders::_4, std::placeholders::_5);

//   for (auto i = 0; i < this->m_strategy.buy_size(); ++i) {
//     const tradingpb::CtrlCondition& cc = this->m_strategy.buy(i);

//     if (cc.indicator() == "weekday") {
//       this->onWeekDay(cc, issim, ts, index, &asset, f);
//     } else if (cc.indicator() == "monthday") {
//       this->onWeekDay(cc, issim, ts, index, &asset, f);
//     } else if (cc.indicator() == "buyandhold") {
//       this->onBuyAndHold(cc, issim, ts, index, &asset, f);
//     }
//   }
// }

// void Strategy::onWeekDay(const tradingpb::CtrlCondition& cc, bool issim,
//                          TimeStamp ts, int index,
//                          const tradingpb::Asset* pAsset,
//                          Strategy::FuncOnCtrl onctrl) {
//   tm ctm;
//   timestamp2timeUTC(ts, ctm);

//   if (ctm.tm_wday == cc.vals(0)) {
//     onctrl(issim, ts, index, pAsset, cc.vals(1), cc.vals(2));
//   }
// }

// void Strategy::onMonthDay(const tradingpb::CtrlCondition& cc, bool issim,
//                           TimeStamp ts, int index,
//                           const tradingpb::Asset* pAsset,
//                           Strategy::FuncOnCtrl onctrl) {
//   tm ctm;
//   timestamp2timeUTC(ts, ctm);

//   if (ctm.tm_mday == cc.vals(0)) {
//     onctrl(issim, ts, index, pAsset, cc.vals(1), cc.vals(2));
//   }
// }

// void Strategy::onBuyAndHold(const tradingpb::CtrlCondition& cc, bool issim,
//                             TimeStamp ts, int index,
//                             const tradingpb::Asset* pAsset,
//                             Strategy::FuncOnCtrl onctrl) {
//   if (index == 0) {
//     onctrl(issim, ts, index, pAsset, cc.vals(0), 0);
//   }
// }

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
  this->m_money += money;
  this->m_fee += fee;
  this->m_volume += volume;
  this->m_price = this->m_money / this->m_volume;
}

CR2END