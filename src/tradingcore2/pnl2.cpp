
#include <gsl/statistics/gsl_statistics_float.h>
#include <math.h>
#include <tradingcore2/csv.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/pnl2.h>
#include <tradingcore2/utils.h>
#include <tradingcore2/wallet.h>

#include <functional>

CR2BEGIN

void PNL2::release() { this->m_data.Clear(); }

void PNL2::buyAsset(const char* market0, const char* symbol0, time_t ts,
                    Money money, Money volume) {
  auto objTotal = this->m_data.mutable_total();
  auto ctrl = objTotal->add_lstctrl();

  auto src = ctrl->mutable_src();
  src->set_market("money");
  src->set_code("cny");
  ctrl->set_volumesrc(money);

  auto dst = ctrl->mutable_dst();
  dst->set_market(market0);
  dst->set_code(symbol0);
  ctrl->set_volumedst(volume);

  ctrl->set_ts(ts);
  ctrl->set_type(::tradingpb::CTRL_BUY);
}

void PNL2::sellAsset(const char* market0, const char* symbol0, time_t ts,
                     Money money, Money volume) {
  auto objTotal = this->m_data.mutable_total();
  auto ctrl = objTotal->add_lstctrl();

  auto src = ctrl->mutable_src();
  src->set_market(market0);
  src->set_code(symbol0);
  ctrl->set_volumesrc(volume);

  auto dst = ctrl->mutable_dst();
  dst->set_market("money");
  dst->set_code("cny");
  ctrl->set_volumedst(money);

  ctrl->set_ts(ts);
  ctrl->set_type(::tradingpb::CTRL_SELL);
}

void PNL2::withdraw(Money money, time_t ts) {
  auto objTotal = this->m_data.mutable_total();
  auto ctrl = objTotal->add_lstctrl();

  auto src = ctrl->mutable_src();
  src->set_market("money");
  src->set_code("cny");
  ctrl->set_volumesrc(money);

  ctrl->set_ts(ts);
  ctrl->set_type(::tradingpb::CTRL_WITHDRAW);
}

void PNL2::deposit(Money money, time_t ts) {
  auto objTotal = this->m_data.mutable_total();
  auto ctrl = objTotal->add_lstctrl();

  auto dst = ctrl->mutable_dst();
  dst->set_market("money");
  dst->set_code("cny");
  ctrl->set_volumedst(money);

  ctrl->set_ts(ts);
  ctrl->set_type(::tradingpb::CTRL_DEPOSIT);
}

void PNL2::addTimestamp(time_t ts) {
  
}

void PNL2::initInvest(const Exchange& exchange, Money invest, Money handMoney,
                      TimeStamp tsStart, TimeStamp tsEnd) {
  auto f = std::bind(&PNL2::onInitInvestTimeStamp, this, std::placeholders::_1,
                     std::placeholders::_2, invest, handMoney);
  exchange.forEachTimeStamp(f, tsStart, tsEnd);
}

void PNL2::onInitInvestTimeStamp(const Exchange& exchange, TimeStamp ts,
                                 Money invest, Money handMoney) {
  // for (auto it = this->m_lst.begin(); it != this->m_lst.end(); ++it) {
  //   if (ts == it->ts) {
  //     it->invest = invest;
  //     it->curMoney = handMoney;

  //     return;
  //   } else if (ts < it->ts) {
  //     Node n;

  //     n.ts = ts;
  //     n.invest = invest;
  //     n.curMoney = handMoney;

  //     this->m_lst.insert(it, n);

  //     return;
  //   }
  // }

  // this->pushData(ts, invest, handMoney);
}

CR2END