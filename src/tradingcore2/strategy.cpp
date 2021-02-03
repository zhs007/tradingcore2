
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/pnl2.h>
#include <tradingcore2/strategy.h>
#include <tradingcore2/utils.h>
#include <tradingcore2/wallet.h>

#include <functional>

CR2BEGIN

void Strategy::simulateTrading() {
  std::set<std::string> indicators;
  CtrlConditionMgr::getSingleton()->getIndicators(indicators, *this);

  for (auto it = indicators.begin(); it != indicators.end(); ++it) {
    this->m_mapIndicators.addIndicator(it->c_str(),
                                       this->m_strategy.asset().code().c_str());
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
                               Volume volume, TimeStamp ts, int index,
                               int strategyID, int ctrlConditionID,
                               int moneyParts) {
  if (this->m_curStopLossPrice > 0) {
    if (curPrice <= this->m_curStopLossPrice) {
      m_stoplossNums++;

      Money fee = 0;
      auto f = std::bind(&Strategy::calcFee4Sell, this, std::placeholders::_1,
                         std::placeholders::_2, std::placeholders::_3,
                         std::placeholders::_4);

      auto money =
          this->m_wallet.sellAssets(assetsName, volume, fee, ts, strategyID,
                                    ctrlConditionID, f, moneyParts);

      return money;
    }
  }

  return 0;
}

Money Strategy::calcFee4Buy(const char* assetsName, Money money, Volume volume,
                            TimeStamp ts) {
  auto buy = this->m_strategy.feebuy();

  if (buy.percentage() > 0) {
    Money fee = money * buy.percentage();

    if (buy.minlimit() > 0 && fee < buy.minlimit()) {
      fee = buy.minlimit();
    }

    if (buy.maxlimit() > 0 && fee > buy.maxlimit()) {
      fee = buy.maxlimit();
    }

    return fee;
  }

  return 0;
}

Money Strategy::calcFee4Sell(const char* assetsName, Money money, Volume volume,
                             TimeStamp ts) {
  auto sell = this->m_strategy.feesell();

  if (sell.percentage() > 0) {
    Money fee = money * sell.percentage();

    if (sell.minlimit() > 0 && fee < sell.minlimit()) {
      fee = sell.minlimit();
    }

    if (sell.maxlimit() > 0 && fee > sell.maxlimit()) {
      fee = sell.maxlimit();
    }

    return fee;
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

void Strategy::buy(bool issim, TimeStamp ts, int strategyID,
                   int ctrlConditionID, bool noNextTimes) {
  auto buy = this->m_strategy.paramsbuy();
  auto f = std::bind(&Strategy::calcFee4Buy, this, std::placeholders::_1,
                     std::placeholders::_2, std::placeholders::_3,
                     std::placeholders::_4);

  if (!noNextTimes && buy.nexttimes() > 0) {
    this->nextBuy(buy.nexttimes(), strategyID, ctrlConditionID);
  } else if (buy.perinitmoney() > 0) {
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

    this->onBuy(issim, ts, m, volume, fee, 0);

    this->m_wallet.buyAssets(this->m_strategy.asset().code().c_str(), m, fee,
                             ts, strategyID, ctrlConditionID, f, 0);
  } else if (buy.perhandmoney() > 0) {
    // LOG(INFO) << "buy " << this->m_handMoney;

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

    this->onBuy(issim, ts, m, volume, fee, 0);

    this->m_wallet.buyAssets(this->m_strategy.asset().code().c_str(), m, fee,
                             ts, strategyID, ctrlConditionID, f, 0);
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

    this->onBuy(issim, ts, m, volume, fee, 0);

    this->m_wallet.buyAssets(this->m_strategy.asset().code().c_str(), m, fee,
                             ts, strategyID, ctrlConditionID, f, 0);
  } else if (buy.moneyparts() > 0) {
    if (this->m_lastMoneyParts < 0) {
      this->m_lastMoneyParts = buy.moneyparts();
    }

    if (this->m_lastMoneyParts <= 0) {
      return;
    }

    auto m = this->m_handMoney / this->m_lastMoneyParts;

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

    this->onBuy(issim, ts, m, volume, fee, -1);

    this->m_wallet.buyAssets(this->m_strategy.asset().code().c_str(), m, fee,
                             ts, strategyID, ctrlConditionID, f, 1);
  }
}

void Strategy::onBuy(bool issim, TimeStamp ts, Money money, Volume volume,
                     Money fee, int offMoneyParts) {
  assert(money <= this->m_handMoney);

  this->m_handMoney -= money;
  this->m_costMoney += money;
  this->m_fee += fee;
  this->m_volume += volume;
  this->m_price = this->m_costMoney / this->m_volume;

  this->m_lastMoneyParts += offMoneyParts;
}

void Strategy::sell(bool issim, TimeStamp ts, int strategyID,
                    int ctrlConditionID, bool noNextTimes) {
  auto sell = this->m_strategy.paramssell();
  auto f = std::bind(&Strategy::calcFee4Sell, this, std::placeholders::_1,
                     std::placeholders::_2, std::placeholders::_3,
                     std::placeholders::_4);

  if (!noNextTimes && sell.nexttimes() > 0) {
    this->nextSell(sell.nexttimes(), strategyID, ctrlConditionID);
  } else if (sell.volume() > 0) {
    int moneyParts;
    auto v1 = this->m_wallet.calcAssetVolume(
        this->m_strategy.asset().code().c_str(), ts, moneyParts);

    auto v = sell.volume();
    if (v > this->m_volume) {
      v = this->m_volume;
    }

    if (v > 0) {
      assert(v == v1);

      Money fee = 0;

      auto m = this->m_wallet.sellAssets(
          this->m_strategy.asset().code().c_str(), v, fee, ts, strategyID,
          ctrlConditionID, f, moneyParts);

      this->onSell(issim, ts, m, v, 0, moneyParts);
    }
  } else if (sell.pervolume() > 0) {
    int moneyParts;
    auto v1 = this->m_wallet.calcAssetVolume(
        this->m_strategy.asset().code().c_str(), ts, moneyParts);

    auto v = sell.pervolume() * this->m_volume;

    if (v > 0) {
      assert(v == v1);

      Money fee = 0;

      auto m = this->m_wallet.sellAssets(
          this->m_strategy.asset().code().c_str(), v, fee, ts, strategyID,
          ctrlConditionID, f, moneyParts);

      this->onSell(issim, ts, m, v, 0, moneyParts);
    }

    // LOG(INFO) << "sell " << this->m_handMoney;
  } else if (sell.money() > 0) {
  } else if (sell.keeptime() > 0) {
    int moneyParts;
    auto v = this->m_wallet.calcAssetVolumeWithKeepTime(
        this->m_strategy.asset().code().c_str(), sell.keeptime(), ts,
        moneyParts);

    if (v > 0) {
      Money fee = 0;
      auto m = this->m_wallet.sellAssets(
          this->m_strategy.asset().code().c_str(), v, fee, ts, strategyID,
          ctrlConditionID, f, moneyParts);

      this->onSell(issim, ts, m, v, 0, moneyParts);
    }
  }
}

void Strategy::onSell(bool issim, TimeStamp ts, Money money, Volume volume,
                      Money fee, int offMoneyParts) {
  assert(volume <= this->m_volume);

  this->m_handMoney += money;
  this->m_fee += fee;
  this->m_volume -= volume;

  if (this->m_volume > 0) {
    this->m_costMoney = this->m_price * this->m_volume;
  } else {
    this->m_costMoney = 0;
  }

  this->m_lastMoneyParts += offMoneyParts;
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

void Strategy::nextBuy(int times, int strategyID, int ctrlConditionID) {
  this->m_lastTimesBuy = times;
  this->m_nextBuyStrategyID = strategyID;
  this->m_nextBuyCtrlConditionID = ctrlConditionID;
}

void Strategy::nextSell(int times, int strategyID, int ctrlConditionID) {
  this->m_lastTimesSell = times;
  this->m_nextSellStrategyID = strategyID;
  this->m_nextSellCtrlConditionID = ctrlConditionID;
}

void Strategy::onNextTimes(bool issim, TimeStamp ts) {
  bool canbuy = false;
  bool cansell = false;
  if (this->m_lastTimesBuy > 0) {
    this->m_lastTimesBuy--;

    if (this->m_lastTimesBuy <= 0) {
      canbuy = true;
    }
  }

  if (this->m_lastTimesSell > 0) {
    this->m_lastTimesSell--;

    if (this->m_lastTimesSell <= 0) {
      cansell = true;
    }
  }

  if (canbuy && !cansell) {
    this->buy(issim, ts, this->m_nextBuyStrategyID,
              this->m_nextBuyCtrlConditionID, true);
  } else if (cansell && !canbuy) {
    this->sell(issim, ts, this->m_nextSellStrategyID,
               this->m_nextSellCtrlConditionID, true);
  }
}

void Strategy::buildIndicators(const tradingpb::SimTradingParams& params,
                               PNL2& pnl2) const {
  if (params.indicators_size() > 0) {
    for (auto i = 0; i < params.indicators_size(); ++i) {
      auto ci = params.indicators(i);
      auto pIndicator = this->m_mapIndicators.getIndicator(ci.c_str());
      if (pIndicator != NULL) {
        auto curasset = params.assets(params.mainassetindex());

        auto pDI = pnl2.m_data.mutable_total()->add_indicators();
        pDI->set_fullname(ci.c_str());
        pDI->set_type(tradingpb::IT_SINGLEPRICE);

        for (auto j = 0; j < pIndicator->getLength(); ++j) {
          auto cv = pIndicator->getSingleValue(j);
          assert(cv != NULL);

          CandleData cd;
          this->m_exchange.getData(curasset.code().c_str(), j, cd);

          auto pDID = pDI->add_data();
          pDID->set_ts(cd.ts);

          pDID->add_vals(cv->value);
        }
      }
    }
  }
}

CR2END