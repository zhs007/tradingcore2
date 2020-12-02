
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/strategy.h>
#include <tradingcore2/utils.h>
#include <tradingcore2/wallet.h>

#include <functional>

CR2BEGIN

void Strategy::simulateTrading() {
  std::set<std::string> indicators;
  CtrlConditionMgr::getSingleton()->getIndicators(indicators, *this);

  for (auto it = indicators.begin(); it != indicators.end(); ++it) {
    this->m_mapIndicators.addIndicator(it->c_str());
  }

  this->m_mapIndicators.build(this->m_exchange,
                              this->m_strategy.asset().code().c_str());

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
  if (index == 0) {
    this->initMoney(issim, ts);
  }

  this->onAIP(issim, ts);

  CtrlConditionMgr::getSingleton()->procStrategy(*this, this->m_pCCData, issim,
                                                 ts, index);
}

void Strategy::buy(bool issim, TimeStamp ts) {
  auto buy = this->m_strategy.paramsbuy();

  if (buy.perinitmoney() > 0) {
    auto m = this->m_initMoney * buy.perinitmoney();

    if (m <= 0) {
      return;
    }

    Volume volume = ZEROVOLUME;
    Money price = ZEROMONEY;
    Money fee = ZEROMONEY;

    bool isok = m_exchange.calculateVolume(
        this->m_strategy.asset().code().c_str(), ts, m, volume, price, fee);
    assert(isok);
    assert(price > ZEROMONEY);

    this->onBuy(issim, ts, m, volume, fee);

    this->m_wallet.buyAssets(this->m_strategy.asset().code().c_str(), m, ts);
  } else if (buy.perhandmoney() > 0) {
    auto m = this->m_handMoney * buy.perhandmoney();

    if (m <= 0) {
      return;
    }

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

    if (m <= 0) {
      return;
    }

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

void Strategy::initMoney(bool issim, TimeStamp ts) {
  auto init = this->m_strategy.paramsinit();

  this->m_initMoney = init.money();
  if (this->m_initMoney > 0) {
    this->m_wallet.deposit(this->m_initMoney, ts);
    this->m_handMoney = this->m_initMoney;
  }
}

void Strategy::onAIP(bool issim, TimeStamp ts) {
  if (this->m_strategy.has_paramsaip()) {
    auto aip = this->m_strategy.paramsaip();
    if (aip.type() == tradingpb::AIPTT_WEEKDAY) {
      auto weekoff = calcWeekOffWithWeekDay(this->m_lastAIPTs, ts, aip.day());
      if (weekoff > 0) {
        this->m_wallet.deposit(aip.money() * weekoff, ts);
        this->m_handMoney += aip.money();
        this->m_lastAIPTs = ts;
      }
    } else if (aip.type() == tradingpb::AIPTT_MONTHDAY) {
      auto monthoff =
          calcMonthOffWithMonthDay(this->m_lastAIPTs, ts, aip.day());
      if (monthoff > 0) {
        this->m_wallet.deposit(aip.money() * monthoff, ts);
        this->m_handMoney += aip.money() * monthoff;
        this->m_lastAIPTs = ts;
      }
    }
  }
}

void Strategy::release() { this->m_mapIndicators.release(); }

CR2END