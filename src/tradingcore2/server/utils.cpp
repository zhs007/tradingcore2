#include "utils.h"

CR2BEGIN

void setTradeData(tradingcore2pb::TradeData& td,
                  const WalletHistoryNode& node) {
  assert(node.nodeType == WHNT_TRADE);

  td.set_tradetype(node.trade.tradeType == TT_BUY ? tradingcore2pb::TT_BUY
                                                  : tradingcore2pb::TT_SELL);
  td.set_assetsname(node.trade.assetsName);
  td.set_money(node.trade.money);
  td.set_volume(node.trade.volume);
  td.set_price(node.trade.price);
  td.set_fee(node.trade.fee);
  td.set_ts(node.trade.ts);
  td.set_offmoney(node.offMoney);
}

void setWant2TradeData(tradingcore2pb::TradeData& td,
                       const WalletHistoryNode& node) {
  assert(node.nodeType == WHNT_WANTTOTRADE);

  td.set_tradetype(node.trade.tradeType == TT_BUY ? tradingcore2pb::TT_BUY
                                                  : tradingcore2pb::TT_SELL);
  td.set_assetsname(node.trade.assetsName);
  td.set_money(node.trade.money);
  td.set_volume(node.trade.volume);
  // td.set_price(node.trade.price);
  // td.set_fee(node.trade.fee);
  td.set_ts(node.trade.ts);
  // td.set_offmoney(node.offMoney);
  td.set_price(node.trade.limitPrice);
}

void setTradeHistory(tradingcore2pb::TrainNodeResult& tnr,
                     const Wallet& wallet) {
  auto onhistory = [&tnr, &wallet](const WalletHistoryNode& node) {
    if (node.nodeType == WHNT_TRADE) {
      auto td = tnr.add_history();
      setTradeData(*td, node);
    }
    // else if (node.nodeType == WHNT_WANTTOTRADE) {
    //   auto td = tnr.add_history();
    //   setWant2TradeData(*td, node);
    // }
  };

  wallet.forEachHistory(onhistory);
}

CR2END