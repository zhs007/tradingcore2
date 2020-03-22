
#include <tradingcore2/exchange.h>
#include <tradingcore2/strategy.h>

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

CR2END