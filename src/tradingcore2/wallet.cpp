
#include <tradingcore2/exchange.h>
#include <tradingcore2/pnl.h>
#include <tradingcore2/pnl2.h>
#include <tradingcore2/wallet.h>

CR2BEGIN

Wallet::Wallet(Exchange& exchange)
    : m_exchange(exchange), m_money(ZEROMONEY), m_cost(ZEROMONEY) {}

Assets Wallet::getAssets(const char* assetsName) const {
  auto assets = this->m_map.getAssets(assetsName);
  if (assets == NULL) {
    return Assets();
  }

  return *assets;
}

void Wallet::deposit(Money money, TimeStamp ts) {
  assert(money > ZEROMONEY);

  this->m_money += money;
  this->m_cost += money;

  WalletHistoryNode n;
  n.setDeposit(money, ts);

  this->_addHistory(n);
}

void Wallet::withdraw(Money money, TimeStamp ts) {
  assert(money > ZEROMONEY);

  if (money > this->m_money) {
    money = this->m_money;
  }

  if (money == 0) {
    return;
  }

  this->m_money -= money;
  this->m_cost -= money;

  WalletHistoryNode n;
  n.setWithdraw(-money, ts);

  this->_addHistory(n);
}

Volume Wallet::buyAssets(const char* assetsName, Money money, Money& fee,
                         TimeStamp ts, int strategyID, int ctrlConditionID,
                         FuncCalcFee calcFee, int moneyParts) {
  assert(assetsName != NULL);
  assert(money > ZEROMONEY);

  if (money > this->m_money) {
    money = this->m_money;
  }

  Volume volume = ZEROVOLUME;
  Money price = ZEROMONEY;
  // Money fee = fee;

  bool isok = m_exchange.calculateVolume(assetsName, ts, money, volume, price,
                                         fee, calcFee);
  assert(isok);
  assert(price > ZEROMONEY);

  this->m_map.buyAssets(assetsName, ts, price, volume, fee);

  if (calcFee != NULL) {
    fee = calcFee(assetsName, money, volume, ts);
  }

  this->m_money -= money;
  this->m_money -= fee;

  WalletHistoryNode n;
  n.setTrade(TT_BUY, assetsName, price, volume, fee, ts, -money, strategyID,
             ctrlConditionID, moneyParts);

  this->_addHistory(n);

  // LOG(INFO) << "TT_BUY " << this->m_history.size() << " " << ts;

  return volume;
}

Volume Wallet::buyAssets2(const char* assetsName, Money money, Volume volume,
                          Money fee, TimeStamp ts, int strategyID,
                          int ctrlConditionID, int moneyParts, Money price) {
  assert(assetsName != NULL);
  assert(money > ZEROMONEY);
  assert(price > ZEROMONEY);

  if (money > this->m_money) {
    money = this->m_money;
  }

  // Volume volume = ZEROVOLUME;
  // Money price = ZEROMONEY;
  // Money fee = fee;

  // bool isok =
  //     m_exchange.calculateVolume(assetsName, ts, money, volume, price, fee);
  // assert(isok);
  // assert(price > ZEROMONEY);

  this->m_map.buyAssets(assetsName, ts, price, volume, fee);

  // if (calcFee != NULL) {
  //   fee = calcFee(assetsName, money, volume, ts);
  // }

  this->m_money -= money;
  this->m_money -= fee;

  WalletHistoryNode n;
  n.setTrade(TT_BUY, assetsName, price, volume, fee, ts, -money, strategyID,
             ctrlConditionID, moneyParts);

  this->_addHistory(n);

  // LOG(INFO) << "TT_BUY " << this->m_history.size() << " " << ts;

  return volume;
}

Money Wallet::sellAssets(const char* assetsName, Volume volume, Money& fee,
                         TimeStamp ts, int strategyID, int ctrlConditionID,
                         FuncCalcFee calcFee, int moneyParts) {
  assert(assetsName != NULL);
  assert(volume > ZEROVOLUME);

  auto assets = this->m_map.getAssets(assetsName);
  assert(assets != NULL);

  if (volume > assets->volume) {
    volume = assets->volume;
  }

  Money money = ZEROMONEY;
  Money price = ZEROMONEY;
  // Money fee = ZEROMONEY;

  bool isok =
      m_exchange.calculatePrice(assetsName, ts, volume, money, price, fee);
  assert(isok);

  this->m_map.sellAssets(assetsName, ts, price, volume, fee);

  if (calcFee != NULL) {
    fee = calcFee(assetsName, money, volume, ts);
  }

  this->m_money += money;
  this->m_money -= fee;

  WalletHistoryNode n;
  n.setTrade(TT_SELL, assetsName, price, volume, fee, ts, money, strategyID,
             ctrlConditionID, moneyParts);

  this->_addHistory(n);

  // LOG(INFO) << "TT_SELL " << this->m_history.size() << " " << ts;

  return money;
}

Money Wallet::sellAssets2(const char* assetsName, Volume volume, Money& fee,
                          TimeStamp ts, int strategyID, int ctrlConditionID,
                          FuncCalcFee calcFee, int moneyParts,
                          Money limitPrice) {
  assert(assetsName != NULL);
  assert(volume > ZEROVOLUME);

  auto assets = this->m_map.getAssets(assetsName);
  assert(assets != NULL);

  if (volume > assets->volume) {
    volume = assets->volume;
  }

  if (volume <= ZEROVOLUME) {
    fee = ZEROMONEY;

    return ZEROMONEY;
  }

  Money money = ZEROMONEY;
  Money price = ZEROMONEY;
  // Money fee = ZEROMONEY;

  bool isok = m_exchange.calculatePriceWithLimitPrice(
      assetsName, ts, volume, money, price, fee, limitPrice, calcFee);
  if (!isok) {
    fee = ZEROMONEY;

    return ZEROMONEY;
  }

  this->m_map.sellAssets(assetsName, ts, price, volume, fee);

  // if (calcFee != NULL) {
  //   fee = calcFee(assetsName, money, volume, ts);
  // }

  this->m_money += money;
  this->m_money -= fee;

  WalletHistoryNode n;
  n.setTrade(TT_SELL, assetsName, price, volume, fee, ts, money, strategyID,
             ctrlConditionID, moneyParts);

  this->_addHistory(n);

  // LOG(INFO) << "TT_SELL " << this->m_history.size() << " " << ts;

  return money;
}

// forEachHistory - foreach history
void Wallet::forEachHistory(Wallet::FuncOnHistory func) const {
  for (auto it = this->m_history.begin(); it != this->m_history.end(); ++it) {
    func(*it);
  }
}

void Wallet::buildPNL(PNL& pnl) const {
  AssetsMap map;
  Money invest = ZEROMONEY;
  Money handMoney = ZEROMONEY;

  auto preit = this->m_history.end();
  for (auto it = this->m_history.begin(); it != this->m_history.end(); ++it) {
    if (preit != this->m_history.end() && preit->ts < it->ts) {
      map.makePNL(pnl, this->m_exchange, invest, handMoney, preit->ts, it->ts);
    }

    if (it->nodeType == WHNT_DEPOSIT) {
      invest += it->offMoney;
    } else if (it->nodeType == WHNT_WITHDRAW) {
      invest += it->offMoney;
    } else if (it->nodeType == WHNT_TRADE) {
      if (it->trade.tradeType == TT_BUY) {
        map.buyAssets(it->trade.assetsName.c_str(), it->ts, it->trade.price,
                      it->trade.volume, it->trade.fee);
      } else {
        map.sellAssets(it->trade.assetsName.c_str(), it->ts, it->trade.price,
                       it->trade.volume, it->trade.fee);
      }
    } else {
      assert(false && "Wallet::buildPNL invalid nodeType");
    }

    handMoney += it->offMoney;

    preit = it;
  }

  //! 这里之所以 getLastTimeStamp() + 1
  //! 是因为makePNL不计算终点，这里要多算一步，才能吧终点也算进去
  map.makePNL(pnl, this->m_exchange, invest, handMoney, preit->ts,
              this->m_exchange.getLastTimeStamp() + 1);

  //! 最后还要处理买卖信息，需要重新遍历一次
  for (auto it = this->m_history.begin(); it != this->m_history.end(); ++it) {
    if (it->nodeType == WHNT_TRADE) {
      if (it->trade.tradeType == TT_BUY) {
        pnl.setBuy(it->ts, it->trade.money);
      } else {
        pnl.setSell(it->ts, it->trade.money);
      }
    }
  }

  pnl.onBuildEnd(this->m_exchange);
}

void Wallet::buildPNL2(const tradingpb::SimTradingParams& params,
                       PNL2& pnl2) const {
  pnl2.initTimestamp(this->m_exchange);

  LOG(INFO) << "buildPNL2:initTimestamp ok.";

  for (auto it = this->m_history.begin(); it != this->m_history.end(); ++it) {
    if (it->nodeType == WHNT_DEPOSIT) {
      pnl2.deposit(it->offMoney, it->ts);
    } else if (it->nodeType == WHNT_WITHDRAW) {
      pnl2.withdraw(it->offMoney, it->ts);
    } else if (it->nodeType == WHNT_TRADE) {
      if (it->trade.tradeType == TT_BUY) {
        pnl2.buyAsset(this->m_exchange.getMarketName(),
                      it->trade.assetsName.c_str(), it->ts,
                      it->trade.price * it->trade.volume, it->trade.volume,
                      it->trade.fee, it->trade.moneyParts);
      } else {
        pnl2.sellAsset(this->m_exchange.getMarketName(),
                       it->trade.assetsName.c_str(), it->ts,
                       it->trade.price * it->trade.volume, it->trade.volume,
                       it->trade.fee, it->trade.moneyParts);
      }
    } else if (it->nodeType == WHNT_WANTTOTRADE) {
      if (it->trade.tradeType == TT_BUY) {
        pnl2.want2buyAsset(this->m_exchange.getMarketName(),
                           it->trade.assetsName.c_str(), it->ts,
                           it->trade.money, it->trade.limitPrice);
      } else {
        pnl2.want2sellAsset(this->m_exchange.getMarketName(),
                            it->trade.assetsName.c_str(), it->ts,
                            it->trade.volume, it->trade.limitPrice);
      }
    }
  }

  LOG(INFO) << "buildPNL2:proc contorl ok.";

  pnl2.onBuildCtrl(this->m_exchange);
  LOG(INFO) << "buildPNL2:onBuildCtrl ok.";

  // pnl2.procCtrlNodeData(this->m_exchange);
  // LOG(INFO) << "buildPNL2:procCtrlNodeData ok.";

  pnl2.procTotalPNLAssetData(this->m_exchange);
  LOG(INFO) << "buildPNL2:procTotalPNLAssetData ok.";

  pnl2.onBuildEnd(this->m_exchange);

  pnl2.clearCtrlTmpData();

  // if (params.indicators_size() > 0) {
  //   for (auto i = 0; i < params.indicators_size(); ++i) {

  //   }
  // }
}

const WalletHistoryNode* Wallet::getLastNode(TradeType tradeType) const {
  // LOG(INFO) << "getLastNode " << this->m_history.size();

  for (auto it = this->m_history.rbegin(); it != this->m_history.rend(); ++it) {
    if (it->nodeType == WHNT_TRADE && it->trade.tradeType == tradeType) {
      return &(*it);
    }
  }

  return NULL;
}

// calcAssetVolumeWithKeepTime - 计算持有时间超过keeptime的数量
Volume Wallet::calcAssetVolumeWithKeepTime(const char* assetsName,
                                           TimeStamp keeptime, TimeStamp ts,
                                           int& moneyParts) const {
  Volume v = 0;
  moneyParts = 0;

  for (auto it = this->m_history.begin(); it != this->m_history.end(); ++it) {
    if (ts < it->ts) {
      break;
    }

    if (it->nodeType == WHNT_TRADE && it->trade.assetsName == assetsName) {
      if (it->trade.tradeType == TT_BUY) {
        if (ts > it->ts + keeptime) {
          v += it->trade.volume;
          moneyParts += it->trade.moneyParts;
        }
      } else if (it->trade.tradeType == TT_SELL) {
        v -= it->trade.volume;
        moneyParts -= it->trade.moneyParts;
      }
    }
  }

  if (v < 0) {
    return 0;
  }

  return v;
}

// calcAssetVolume - 计算持有时间超过keeptime的数量
Volume Wallet::calcAssetVolume(const char* assetsName, TimeStamp ts,
                               int& moneyParts) const {
  Volume v = 0;
  moneyParts = 0;

  for (auto it = this->m_history.begin(); it != this->m_history.end(); ++it) {
    if (ts < it->ts) {
      break;
    }

    if (it->nodeType == WHNT_TRADE && it->trade.assetsName == assetsName) {
      if (it->trade.tradeType == TT_BUY) {
        v += it->trade.volume;
        moneyParts += it->trade.moneyParts;
      } else if (it->trade.tradeType == TT_SELL) {
        v -= it->trade.volume;
        moneyParts -= it->trade.moneyParts;
      }
    }
  }

  if (v < 0) {
    return 0;
  }

  return v;
}

void Wallet::want2buy(const char* assetsName, Money money, Money limitPrice,
                      TimeStamp ts) {
  WalletHistoryNode n;
  n.setWant2Trade(TT_BUY, assetsName, money, 0, ts, limitPrice);

  this->_addHistory(n);
}

void Wallet::want2sell(const char* assetsName, Volume volume, Money limitPrice,
                       TimeStamp ts) {
  WalletHistoryNode n;
  n.setWant2Trade(TT_SELL, assetsName, 0, volume, ts, limitPrice);

  this->_addHistory(n);
}

CR2END