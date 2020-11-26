
#include <tradingcore2/exchange.h>
#include <tradingcore2/strategy.h>
#include <tradingcore2/wallet.h>

#include <functional>

CR2BEGIN

void Strategy::simulateTrading() {
  auto f = std::bind(&Strategy::onSimulateTradingTimeStamp, this,
                     std::placeholders::_2, std::placeholders::_3);

  this->m_exchange.forEachTimeStamp(f, 0, 0);
}

void Strategy::onSimulateTradingTimeStamp(TimeStamp ts, int index) {
  this->onTimeStamp(ts, index);
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

bool Strategy::onCtrlConditionBuy(TimeStamp ts, int index) { return false; }

CR2END