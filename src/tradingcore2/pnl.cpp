#include <assert.h>
#include <tradingcore2/exchange.h>
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

void PNL::initInvest(const Exchange& exchange, Money invest, Money handMoney,
                     TimeStamp tsStart, TimeStamp tsEnd) {
  auto f = std::bind(&PNL::onInitInvestTimeStamp, this, std::placeholders::_1,
                     std::placeholders::_2, invest, handMoney);
  exchange.forEachTimeStamp(f, tsStart, tsEnd);
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
  assert(invest > 0);

  Node n;

  n.ts = ts;
  n.invest = invest;
  n.curMoney = curMoney;
  n.percentage = (curMoney - invest) / invest;

  this->m_lst.push_back(n);
}

void PNL::chgData(TimeStamp ts, Money offInvest, Money offMoney) {
  for (auto it = this->m_lst.begin(); it != this->m_lst.end(); ++it) {
    if (ts == it->ts) {
      it->invest += offInvest;
      it->curMoney += offMoney;

      it->percentage = (it->curMoney - it->invest) / it->invest;

      return;
    } else if (ts < it->ts) {
      assert(offInvest > 0);

      Node n;

      n.ts = ts;
      n.invest = offInvest;
      n.curMoney = offMoney;
      n.percentage = (n.curMoney - n.invest) / n.invest;

      this->m_lst.insert(it, n);

      return;
    }
  }

  assert(offInvest > 0);
  this->pushData(ts, offInvest, offMoney);
}

void PNL::onInitInvestTimeStamp(const Exchange& exchange, TimeStamp ts,
                                Money invest, Money handMoney) {
  for (auto it = this->m_lst.begin(); it != this->m_lst.end(); ++it) {
    if (ts == it->ts) {
      it->invest = invest;
      it->curMoney = handMoney;

      return;
    } else if (ts < it->ts) {
      Node n;

      n.ts = ts;
      n.invest = invest;
      n.curMoney = handMoney;

      this->m_lst.insert(it, n);

      return;
    }
  }

  this->pushData(ts, invest, handMoney);
}

PNL::Node PNL::getNode(int index) const {
  assert(index >= 0);
  assert(index < this->m_lst.size());

  return this->m_lst[index];
}

CR2END