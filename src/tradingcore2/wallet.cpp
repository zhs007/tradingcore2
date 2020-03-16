
#include <tradingcore2/exchange.h>
#include <tradingcore2/pnl.h>
#include <tradingcore2/wallet.h>

CR2BEGIN

Wallet::Wallet(Exchange& exchange) : m_exchange(exchange) {}

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

  WalletHistoryNode n;
  n.setDeposit(money, ts);

  this->_addHistory(n);
}

void Wallet::withdraw(Money money, TimeStamp ts) {
  assert(money > ZEROMONEY);

  if (money > this->m_money) {
    money = this->m_money;
  }

  this->m_money -= money;

  WalletHistoryNode n;
  n.setWithdraw(-money, ts);

  this->_addHistory(n);
}

void Wallet::buyAssets(const char* assetsName, Money money, TimeStamp ts) {
  assert(assetsName != NULL);
  assert(money > ZEROMONEY);

  if (money > this->m_money) {
    money = this->m_money;
  }

  Volume volume = ZEROVOLUME;
  Money price = ZEROMONEY;
  Money fee = ZEROMONEY;

  bool isok =
      m_exchange.calculateVolume(assetsName, ts, money, volume, price, fee);
  assert(isok);
  assert(price > ZEROMONEY);

  this->m_map.buyAssets(assetsName, ts, price, volume, fee);

  this->m_money -= money;

  WalletHistoryNode n;
  n.setTrade(TT_BUY, assetsName, price, fee, volume, ts, -money);

  this->_addHistory(n);
}

void Wallet::sellAssets(const char* assetsName, Volume volume, TimeStamp ts) {
  assert(assetsName != NULL);
  assert(volume > ZEROVOLUME);

  auto assets = this->m_map.getAssets(assetsName);
  assert(assets != NULL);

  if (volume > assets->volume) {
    volume = assets->volume;
  }

  Money money = ZEROMONEY;
  Money price = ZEROMONEY;
  Money fee = ZEROMONEY;

  bool isok =
      m_exchange.calculatePrice(assetsName, ts, volume, money, price, fee);
  assert(isok);

  this->m_map.sellAssets(assetsName, ts, price, volume, fee);

  this->m_money += money;

  WalletHistoryNode n;
  n.setTrade(TT_SELL, assetsName, price, fee, volume, ts, money);

  this->_addHistory(n);
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
}

// void Wallet::buildTimeLine(Wallet::FuncOnTimeLine func) const {
//   AssetsMap map;
//   Money invest = ZEROMONEY;
//   std::vector<std::string> lstAssetsName;

//   History::const_iterator preit = this->m_history.end();
//   for (auto it = this->m_history.begin(); it != this->m_history.end(); ++it)
//   {
//     if (preit != this->m_history.end() && preit->ts < it->ts) {
//     }

//     if (it->nodeType == WHNT_DEPOSIT) {
//       invest += it->offMoney;

//       func(this->m_exchange, it->ts, map, it->offMoney, 0);
//     } else if (it->nodeType == WHNT_WITHDRAW) {
//       invest += it->offMoney;

//       func(this->m_exchange, it->ts, map, 0, -it->offMoney);
//     } else if (it->nodeType == WHNT_TRADE) {
//       if (it->trade.tradeType == TT_BUY) {
//         map.buyAssets(it->trade.assetsName.c_str(), it->ts, it->trade.price,
//                       it->trade.volume, it->trade.fee);
//       } else {
//         map.sellAssets(it->trade.assetsName.c_str(), it->ts, it->trade.price,
//                        it->trade.volume, it->trade.fee);
//       }

//       func(this->m_exchange, it->ts, map, 0, 0);

//       auto anit = std::find(lstAssetsName.begin(), lstAssetsName.end(),
//                             it->trade.assetsName.c_str());
//       if (anit != lstAssetsName.end()) {
//         lstAssetsName.push_back(it->trade.assetsName.c_str());
//       }
//     } else {
//       assert(false && "Wallet::buildTimeLine invalid nodeType");
//     }

//     preit = it;
//   }
// }

CR2END