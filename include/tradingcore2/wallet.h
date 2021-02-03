#ifndef __TRADINGCORE2_WALLET_H__
#define __TRADINGCORE2_WALLET_H__

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
  int strategyID;
  int ctrlConditionID;

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

  // void setTrade(Trade& trade, Money offMoney, int strategyID,
  //               int ctrlConditionID) {
  //   this->nodeType = WHNT_TRADE;
  //   this->offMoney = offMoney;
  //   this->trade = trade;
  //   this->ts = trade.ts;
  //   this->strategyID = strategyID;
  //   this->ctrlConditionID = ctrlConditionID;
  // }

  void setTrade(TradeType tradeType, const char* assetsName, Money price,
                Volume volume, Money fee, TimeStamp ts, Money offMoney,
                int strategyID, int ctrlConditionID, int moneyParts) {
    this->nodeType = WHNT_TRADE;
    this->offMoney = offMoney;
    this->ts = ts;

    this->trade.tradeType = tradeType;
    this->trade.assetsName = assetsName;
    this->trade.price = price;
    this->trade.fee = fee;
    this->trade.money = offMoney < 0 ? -offMoney : offMoney;
    this->trade.volume = volume;
    this->trade.ts = ts;
    this->strategyID = strategyID;
    this->ctrlConditionID = ctrlConditionID;

    this->trade.moneyParts = moneyParts;
  }
};

typedef std::function<Money(const char* assetsName, Money money, Volume volume,
                            TimeStamp ts)>
    FuncCalcFee;

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

  Volume buyAssets(const char* assetsName, Money money, Money& fee,
                   TimeStamp ts, int strategyID, int ctrlConditionID,
                   FuncCalcFee calcFee, int moneyParts);

  Money sellAssets(const char* assetsName, Volume volume, Money& fee,
                   TimeStamp ts, int strategyID, int ctrlConditionID,
                   FuncCalcFee calcFee, int moneyParts);

  void deposit(Money money, TimeStamp ts);

  void withdraw(Money money, TimeStamp ts);

  // forEachHistory - foreach history
  void forEachHistory(Wallet::FuncOnHistory func) const;

  const Exchange& getExchange() const { return m_exchange; }

  void buildPNL(PNL& pnl) const;

  void buildPNL2(const tradingpb::SimTradingParams& params, PNL2& pnl2) const;

  Money getMoney() const { return m_money; }

  const WalletHistoryNode* getLastNode(TradeType tradeType) const;

  // calcAssetVolumeWithKeepTime - 计算持有时间超过keeptime的数量
  Volume calcAssetVolumeWithKeepTime(const char* assetsName, TimeStamp keeptime,
                                     TimeStamp ts, int& moneyParts) const;

  // calcAssetVolume - 计算持有时间超过keeptime的数量
  Volume calcAssetVolume(const char* assetsName, TimeStamp ts,
                         int& moneyParts) const;

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