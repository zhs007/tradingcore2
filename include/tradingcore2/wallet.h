#ifndef __TRADINGCORE2_WALLET_H__
#define __TRADINGCORE2_WALLET_H__

#include <assert.h>
#include <tradingcore2/assets.h>
#include <tradingcore2/basedef.h>
#include <tradingcore2/trade.h>

#include <functional>
#include <map>
#include <string>
#include <vector>

CR2BEGIN

enum WalletHistoryNodeType {
  WHNT_NONE = 0,
  WHNT_TRADE = 1,
  WHNT_DEPOSIT = 2,
  WHNT_WITHDRAW = 3
};

struct WalletHistoryNode {
  WalletHistoryNodeType nodeType;
  Money offMoney;
  Trade trade;
  TimeStamp ts;

  WalletHistoryNode() : nodeType(WHNT_NONE) {}

  void setDeposit(Money money, TimeStamp ts) {
    assert(money > 0);

    this->nodeType = WHNT_DEPOSIT;
    this->offMoney = money;
    this->ts = ts;
  }

  void setWithdraw(Money money, TimeStamp ts) {
    assert(money < 0);

    this->nodeType = WHNT_WITHDRAW;
    this->offMoney = money;
    this->ts = ts;
  }

  void setTrade(Trade& trade, Money offMoney) {
    this->nodeType = WHNT_TRADE;
    this->offMoney = offMoney;
    this->trade = trade;
    this->ts = trade.ts;
  }

  void setTrade(TradeType tradeType, const char* assetsName, Money price,
                Volume volume, Money fee, TimeStamp ts, Money offMoney) {
    this->nodeType = WHNT_TRADE;
    this->offMoney = offMoney;

    this->trade.assetsName = assetsName;
    this->trade.price = price;
    this->trade.fee = fee;
    this->trade.money = offMoney < 0 ? -offMoney : offMoney;
    this->trade.volume = volume;
    this->trade.ts = ts;
  }
};

// Wallet - This wallet class is implemented for a single exchange
class Wallet {
 public:
  typedef std::vector<WalletHistoryNode> History;

  // FuncOnHistory - for Wallet::forEachHistory
  typedef std::function<void(const WalletHistoryNode& node)> FuncOnHistory;
  // FuncOnTimeLine - for Wallet::buildTimeLine
  typedef std::function<void(const Exchange& exchange, TimeStamp ts,
                             const AssetsMap& mapAssets, Money moneyDeposit,
                             Money moneyWithdraw)>
      FuncOnTimeLine;

 public:
  Wallet(Exchange& exchange);
  ~Wallet() {}

 public:
  Assets getAssets(const char* assetsName) const;

  void buyAssets(const char* assetsName, Money money, TimeStamp ts);

  void sellAssets(const char* assetsName, Volume volume, TimeStamp ts);

  void deposit(Money money, TimeStamp ts);

  void withdraw(Money money, TimeStamp ts);

  // forEachHistory - foreach history
  void forEachHistory(Wallet::FuncOnHistory func) const;

  const Exchange& getExchange() const { return m_exchange; }

  void buildPNL(PNL& pnl) const;

  Money getMoney() const { return m_money; }

 protected:
  void _addHistory(WalletHistoryNode& n) { m_history.push_back(n); }

 protected:
  Exchange& m_exchange;
  AssetsMap m_map;
  Money m_money;
  History m_history;
};

CR2END

#endif  // __TRADINGCORE2_WALLET_H__