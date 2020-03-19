#include <assert.h>
#include <math.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/pnl.h>
#include <tradingcore2/wallet.h>

#include <functional>

CR2BEGIN

void PNL::initInvest(const Exchange& exchange, Money invest, Money handMoney,
                     TimeStamp tsStart, TimeStamp tsEnd) {
  auto f = std::bind(&PNL::onInitInvestTimeStamp, this, std::placeholders::_1,
                     std::placeholders::_2, invest, handMoney);
  exchange.forEachTimeStamp(f, tsStart, tsEnd);
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

void PNL::onBuildEnd(const Exchange& exchange) {
  this->calcMaxDrawdown(exchange);

  this->calcAnnualizedReturns(exchange);

  this->calcAnnualizedVolatility(exchange);

  this->calcSharpe(exchange);
}

void PNL::calcMaxDrawdown(const Exchange& exchange) {
  auto itmin = this->m_lst.end();
  for (auto it = this->m_lst.begin(); it != this->m_lst.end(); ++it) {
    if (itmin == this->m_lst.end()) {
      itmin = it;
    } else if (itmin->curMoney >= it->curMoney) {
      itmin = it;
    }
  }

  auto itmax = this->m_lst.end();
  for (auto it = this->m_lst.begin(); it != itmin; ++it) {
    if (itmax == this->m_lst.end()) {
      itmax = it;
    } else if (itmax->curMoney < it->curMoney) {
      itmax = it;
    }
  }

  this->m_maxDrawdown = (itmax->curMoney - itmin->curMoney) / itmax->curMoney;
}

void PNL::calcSharpe(const Exchange& exchange) {}

void PNL::calcAnnualizedReturns(const Exchange& exchange) {
  this->m_annualizedReturns =
      (this->m_lst.back().curMoney / this->m_lst.begin()->curMoney - 1) /
      this->m_lst.size() * exchange.getTradingDays4Year();
}

void PNL::calcAnnualizedVolatility(const Exchange& exchange) {
  float* pU = new float[this->m_lst.size() - 1];

  for (int i = 1; i < this->m_lst.size(); ++i) {
    pU[i - 1] = log(this->m_lst[i].curMoney / this->m_lst[i - 1].curMoney);
  }

  delete pU;
}

CR2END