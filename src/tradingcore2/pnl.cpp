#include <assert.h>
#include <tradingcore2/pnl.h>
#include <tradingcore2/wallet.h>
#include <functional>

CR2BEGIN

bool PNL::buildData(const Wallet& wallet, TimeStamp startts, TimeStamp endts) {
  this->release();

  // auto f = std::bind(&PNL::onHistoryNode, this, std::placeholders::_1);
  // wallet.forEachHistory(f);

  return false;
}

void PNL::onHistoryNode(const WalletHistoryNode& node) {
  if (node.nodeType == WHNT_DEPOSIT) {
  } else if (node.nodeType == WHNT_WITHDRAW) {
  } else if (node.nodeType == WHNT_TRADE) {
  } else {
    assert(false && "PNL::onHistoryNode invalid nodeType");
  }
}

void PNL::pushData(TimeStamp ts, Money invest, Money curMoney) {
  Node n;

  n.ts = ts;
  n.invest = invest;
  n.curMoney = curMoney;
  n.percentage = (curMoney - invest) / invest;

  this->m_lst.push_back(n);
}

CR2END