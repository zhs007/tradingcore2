#ifndef __TRADINGCORE2_WALLET_H__
#define __TRADINGCORE2_WALLET_H__

#include <assert.h>
#include <tradingcore2/basedef.h>
#include <tradingcore2/trade.h>
#include <map>
#include <string>
#include <vector>

CR2BEGIN

struct AssetsBlock {
  Volume volume;
  Money inPrice;
  Money feesPaid;
  TimeStamp ts;
};

struct Assets {
  std::string name;
  Volume volume;
  Money inPrice;
  Money feesPaid;
  std::vector<AssetsBlock> blocks;

  Assets() : volume(ZEROVOLUME), inPrice(ZEROMONEY), feesPaid(ZEROMONEY) {}
};

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

  WalletHistoryNode() : nodeType(WHNT_NONE) {}

  void setDeposit(Money money) {
    assert(money > 0);

    this->nodeType = WHNT_DEPOSIT;
    this->offMoney = money;
  }

  void setWithdraw(Money money) {
    assert(money < 0);

    this->nodeType = WHNT_WITHDRAW;
    this->offMoney = money;
  }

  void setTrade(Trade& trade, Money offMoney) {
    this->nodeType = WHNT_TRADE;
    this->offMoney = offMoney;
    this->trade = trade;
  }

  void setTrade(TradeType tradeType, const char* assetsName, Money money,
                Volume volume, TimeStamp ts, Money offMoney) {
    this->nodeType = WHNT_TRADE;
    this->offMoney = offMoney;

    this->trade.assetsName = assetsName;
    this->trade.money = money;
    this->trade.volume = volume;
    this->trade.ts = ts;
  }
};

class Wallet {
 public:
  typedef std::map<std::string, Assets> Map;
  typedef std::pair<std::string, Assets> Pair;
  typedef std::vector<WalletHistoryNode> History;

 public:
  Wallet(Exchange& exchange);
  ~Wallet() {}

 public:
  Assets getAssets(const char* assetsName) const;

  void buyAssets(const char* assetsName, Money money, TimeStamp ts);

  void sellAssets(const char* assetsName, Volume volume, TimeStamp ts);

  void deposit(Money money);

  void withdraw(Money money);

 protected:
  Assets* _getAssets(const char* assetsName);

  void _addHistory(WalletHistoryNode& n) { m_history.push_back(n); }

 protected:
  Exchange& m_exchange;
  Map m_map;
  Money m_money;
  History m_history;
};

CR2END

#endif  // __TRADINGCORE2_WALLET_H__