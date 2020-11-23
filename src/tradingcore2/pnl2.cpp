
#include <gsl/statistics/gsl_statistics_float.h>
#include <math.h>
#include <tradingcore2/csv.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/pnl2.h>
#include <tradingcore2/trnode2/utils.h>
#include <tradingcore2/utils.h>
#include <tradingcore2/wallet.h>

#include <functional>

CR2BEGIN

void PNL2::release() { this->m_data.Clear(); }

void PNL2::buyAsset(const char* market0, const char* symbol0, time_t ts,
                    Money money, Money volume, Money fee) {
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
  ctrl->set_fee(fee);

  this->addAsset(symbol0);
}

void PNL2::sellAsset(const char* market0, const char* symbol0, time_t ts,
                     Money money, Money volume, Money fee) {
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
  ctrl->set_fee(fee);

  this->addAsset(symbol0);
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

void PNL2::onInitTimeStamp(const Exchange& exchange, TimeStamp ts, int index) {
  insTimestamp(this->m_data.mutable_total(), ts);
}

void PNL2::initTimestamp(const Exchange& exchange) {
  // void(const Exchange&, TimeStamp, int)
  auto f = std::bind(&PNL2::onInitTimeStamp, this, std::placeholders::_1,
                     std::placeholders::_2, std::placeholders::_3);
  exchange.forEachTimeStamp(f, 0, -1);
}

void PNL2::procTimestamp(const Exchange& exchange, TimeStamp ts) {
  for (auto it = this->m_mapAssets.begin(); it != this->m_mapAssets.end();
       ++it) {
    CandleData cd;
    if (exchange.getDataWithTimestamp(it->c_str(), ts, cd)) {
      auto cv = this->getAssetVolume(exchange, it->c_str(), ts);
      // cd.close
    }
  }
}

void PNL2::addAsset(const char* asset) {
  auto it = this->m_mapAssets.find(asset);
  if (it == this->m_mapAssets.end()) {
    this->m_mapAssets.insert(asset);
  }
}

Volume PNL2::getAssetVolume(const Exchange& exchange, const char* asset,
                            TimeStamp ts) {
  auto t = this->m_data.total();
  if (t.lstctrl_size() > 0) {
    Volume cv = 0;

    for (auto i = 0; i < t.lstctrl_size(); ++i) {
      auto cc = t.lstctrl(i);
      if (cc.ts() > ts) {
        break;
      }

      if (cc.dst().code() == asset) {
        cv += cc.volumedst();
      } else if (cc.src().code() == asset) {
        cv -= cc.volumesrc();
      }
    }

    return cv;
  }

  return 0;
}

Money PNL2::getAssetCost(const Exchange& exchange, const char* asset,
                         TimeStamp ts) {
  auto t = this->m_data.total();
  if (t.lstctrl_size() > 0) {
    Money cost = 0;

    for (auto i = 0; i < t.lstctrl_size(); ++i) {
      auto cc = t.lstctrl(i);
      if (cc.ts() > ts) {
        break;
      }

      if (cc.dst().code() == asset) {
        CandleData cd;
        if (exchange.getDataWithTimestamp(asset, cc.ts(), cd)) {
          cost += cd.close * cc.volumedst();
        }
      } else if (cc.src().code() == asset) {
        CandleData cd;
        if (exchange.getDataWithTimestamp(asset, cc.ts(), cd)) {
          cost -= cd.close * cc.volumesrc();
        }
      }
    }

    return cost;
  }

  return 0;
}

void PNL2::setTotalPNLAssetData(const Exchange* pExchange,
                                ::tradingpb::PNLDataValue* pVal) {
  assert(pVal != NULL);

  pVal->set_cost(0);
  pVal->set_value(0);

  for (auto it = this->m_mapAssets.begin(); it != this->m_mapAssets.end();
       ++it) {
    auto v = this->getAssetVolume(*pExchange, it->c_str(), pVal->ts());
    auto c = this->getAssetCost(*pExchange, it->c_str(), pVal->ts());
    pVal->set_cost(c + pVal->cost());

    // LOG(INFO) << "volume " << v;

    CandleData cd;
    if (v > 0 && pExchange->getDataWithTimestamp(it->c_str(), pVal->ts(), cd)) {
      // LOG(INFO) << "close " << cd.close;

      pVal->set_value(cd.close * v + pVal->value());
    }
  }

  if (pVal->cost() < 0) {
    pVal->set_value(pVal->value() - pVal->cost());
    pVal->set_cost(0);
  }

  if (pVal->cost() != 0) {
    pVal->set_pervalue(pVal->value() / pVal->cost());
  }
}

void PNL2::procTotalPNLAssetData(const Exchange& exchange) {
  auto f = std::bind(&PNL2::setTotalPNLAssetData, this, &exchange,
                     std::placeholders::_1);
  foreachPNLDataValue(this->m_data.mutable_total(), f);
}

CR2END