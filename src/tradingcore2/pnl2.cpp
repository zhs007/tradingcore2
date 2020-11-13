
#include <gsl/statistics/gsl_statistics_float.h>
#include <math.h>
#include <tradingcore2/csv.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/pnl2.h>
#include <tradingcore2/utils.h>
#include <tradingcore2/wallet.h>

#include <functional>

CR2BEGIN

void PNL2::release() {
}

void PNL2::initInvest(const Exchange& exchange, Money invest, Money handMoney,
                     TimeStamp tsStart, TimeStamp tsEnd) {
//   auto f = std::bind(&PNL2::onInitInvestTimeStamp, this, std::placeholders::_1,
//                      std::placeholders::_2, invest, handMoney);
//   exchange.forEachTimeStamp(f, tsStart, tsEnd);
}

void PNL2::onInitInvestTimeStamp(const Exchange& exchange, TimeStamp ts,
                                Money invest, Money handMoney) {
//   for (auto it = this->m_lst.begin(); it != this->m_lst.end(); ++it) {
//     if (ts == it->ts) {
//       it->invest = invest;
//       it->curMoney = handMoney;

//       return;
//     } else if (ts < it->ts) {
//       Node n;

//       n.ts = ts;
//       n.invest = invest;
//       n.curMoney = handMoney;

//       this->m_lst.insert(it, n);

//       return;
//     }
//   }

//   this->pushData(ts, invest, handMoney);
}

CR2END