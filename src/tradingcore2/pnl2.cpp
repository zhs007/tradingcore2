
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
  auto f = std::bind(&PNL2::onInitTimeStamp, this, std::placeholders::_1,
                     std::placeholders::_2, std::placeholders::_3);
  exchange.forEachTimeStamp(f, 0, -1);
}

void PNL2::addAsset(const char* asset) {
  auto it = this->m_mapAssets.find(asset);
  if (it == this->m_mapAssets.end()) {
    this->m_mapAssets.insert(asset);
  }
}

void PNL2::getAssetInfo(const Exchange& exchange, const char* asset,
                        TimeStamp ts, Money& cost, Volume& volume) {
  // profit = 0;
  cost = 0;
  volume = 0;

  auto t = this->m_data.total();
  if (t.lstctrl_size() > 0) {
    for (auto i = 0; i < t.lstctrl_size(); ++i) {
      auto cc = t.lstctrl(i);
      if (cc.ts() > ts) {
        break;
      }

      if (cc.dst().code() == asset) {
        volume += cc.volumedst();
        cost += cc.volumesrc();
        // CandleData cd;
        // if (exchange.getDataWithTimestamp(asset, cc.ts(), cd)) {
        //   cost += cd.close * cc.volumedst();
        // }
      } else if (cc.src().code() == asset) {
        auto price = cost / volume;

        volume -= cc.volumesrc();

        CandleData cd;
        if (exchange.getDataWithTimestamp(asset, cc.ts(), cd)) {
          cost = volume * price;
          // cost -= cd.close * cc.volumesrc();

          // profit += (cd.close - price) * cc.volumesrc();
        }
      }
    }
  }
}

void PNL2::getAssetInfo2(const Exchange& exchange, const char* asset,
                         TimeStamp ts, Money& cost, Volume& volume) {
  cost = 0;
  volume = 0;

  auto t = this->m_data.total();
  if (t.lstctrl_size() > 0) {
    for (auto i = 0; i < t.lstctrl_size(); ++i) {
      auto cc = t.lstctrl(i);
      if (cc.ts() > ts) {
        break;
      }

      auto mapsiit = cc.mapassetsinfo().find(asset);
      if (mapsiit != cc.mapassetsinfo().end()) {
        volume = (*mapsiit).second.volume();
        cost = (*mapsiit).second.cost();
      }
    }
  }
}

void PNL2::getHandMoneyEx(const Exchange& exchange, TimeStamp ts, Money& total,
                          Money& last) {
  total = 0;
  last = 0;

  auto t = this->m_data.total();
  if (t.lstctrl_size() > 0) {
    for (auto i = 0; i < t.lstctrl_size(); ++i) {
      auto cc = t.lstctrl(i);
      if (cc.ts() > ts) {
        break;
      }

      if (cc.type() == tradingpb::CTRL_DEPOSIT) {
        total += cc.volumedst();
        last += cc.volumedst();
      } else if (cc.type() == tradingpb::CTRL_WITHDRAW) {
        total -= cc.volumedst();
        last -= cc.volumedst();
      } else if (cc.type() == tradingpb::CTRL_BUY) {
        last -= cc.volumesrc();
      } else if (cc.type() == tradingpb::CTRL_SELL) {
        last += cc.volumedst();
      }
    }
  }
}

void PNL2::getHandMoneyEx2(const Exchange& exchange, TimeStamp ts, Money& total,
                           Money& last) {
  total = 0;
  last = 0;

  auto t = this->m_data.total();
  if (t.lstctrl_size() > 0) {
    for (auto i = 0; i < t.lstctrl_size(); ++i) {
      auto cc = t.lstctrl(i);
      if (cc.ts() > ts) {
        break;
      }

      total = cc.totalmoney();
      last = cc.lastmoney();
    }
  }
}

// 构建ctrl统计数据
void PNL2::onBuildCtrl(const Exchange& exchange) {
  std::map<std::string, tradingpb::CtrlNodeAssetInfo> mapAssetInfo;
  for (auto it = this->m_mapAssets.begin(); it != this->m_mapAssets.end();
       ++it) {
    std::pair<std::string, tradingpb::CtrlNodeAssetInfo> p;
    p.first = it->c_str();
    p.second.set_volume(0);
    p.second.set_cost(0);

    mapAssetInfo.insert(p);
  }

  double total = 0;
  double last = 0;

  auto t = this->m_data.total();
  if (t.lstctrl_size() > 0) {
    for (auto i = 0; i < t.lstctrl_size(); ++i) {
      auto cc = t.lstctrl(i);

      if (cc.type() == tradingpb::CTRL_DEPOSIT) {
        total += cc.volumedst();
        last += cc.volumedst();
      } else if (cc.type() == tradingpb::CTRL_WITHDRAW) {
        total -= cc.volumedst();
        last -= cc.volumedst();
      } else if (cc.type() == tradingpb::CTRL_BUY) {
        last -= cc.volumesrc();
      } else if (cc.type() == tradingpb::CTRL_SELL) {
        last += cc.volumedst();
      }

      auto mcc = t.mutable_lstctrl(i);
      mcc->set_totalmoney(total);
      mcc->set_lastmoney(last);

      {
        auto ait1 = mapAssetInfo.find(cc.dst().code());
        if (ait1 != mapAssetInfo.end()) {
          ait1->second.set_volume(ait1->second.volume() + cc.volumedst());
          ait1->second.set_cost(ait1->second.cost() + cc.volumesrc());

          {
            auto mapai = mcc->mutable_mapassetsinfo();
            auto mapaiobj = (*mapai)[cc.dst().code()];

            mapaiobj.set_volume(ait1->second.volume());
            mapaiobj.set_cost(ait1->second.cost());
          }
        }
      }

      {
        auto ait2 = mapAssetInfo.find(cc.src().code());
        if (ait2 != mapAssetInfo.end()) {
          auto price = 0;
          if (ait2->second.volume() > 0) {
            price = ait2->second.cost() / ait2->second.volume();
          }

          ait2->second.set_volume(ait2->second.volume() - cc.volumesrc());

          CandleData cd;
          if (exchange.getDataWithTimestamp(cc.src().code().c_str(), cc.ts(),
                                            cd)) {
            ait2->second.set_cost(ait2->second.volume() * price);
          }

          {
            auto mapai = mcc->mutable_mapassetsinfo();
            auto mapaiobj = (*mapai)[cc.dst().code()];

            mapaiobj.set_volume(ait2->second.volume());
            mapaiobj.set_cost(ait2->second.cost());
          }
        }
      }
    }
  }
}

// 清理构建PNL时需要的统计数据
void PNL2::clearCtrlTmpData() {
  auto t = this->m_data.total();
  if (t.lstctrl_size() > 0) {
    for (auto i = 0; i < t.lstctrl_size(); ++i) {
      auto mcc = t.mutable_lstctrl(i);

      mcc->clear_mapassetsinfo();
      mcc->clear_lastmoney();
      mcc->clear_totalmoney();
    }
  }
}

void PNL2::setTotalPNLAssetData(const Exchange* pExchange,
                                ::tradingpb::PNLDataValue* pVal) {
  assert(pVal != NULL);

  Money total, last;
  this->getHandMoneyEx2(*pExchange, pVal->ts(), total, last);
  pVal->set_cost(total);
  pVal->set_value(last);

  for (auto it = this->m_mapAssets.begin(); it != this->m_mapAssets.end();
       ++it) {
    Money cost;
    Volume volume;
    this->getAssetInfo2(*pExchange, it->c_str(), pVal->ts(), cost, volume);

    // pVal->set_cost(cost + pVal->cost());

    // 临时代码
    pVal->set_volume(volume);

    // LOG(INFO) << "profit " << profit;

    CandleData cd;
    if (volume > 0 &&
        pExchange->getDataWithTimestamp(it->c_str(), pVal->ts(), cd)) {
      // LOG(INFO) << "close " << cd.close;

      pVal->set_value(cd.close * volume + pVal->value());

      // 临时代码
      pVal->set_price(cd.close);
    } else {
      // if (profit == 0) {
      //   pVal->set_value(cost);
      // } else {
      // pVal->set_value(cost + profit);
      // }
    }
  }

  if (pVal->cost() < 0) {
    pVal->set_value(pVal->value() - pVal->cost());
    pVal->set_cost(0);
  }

  if (pVal->cost() != 0) {
    pVal->set_pervalue(pVal->value() / pVal->cost());
  } else {
    pVal->set_pervalue(1);
  }
}

void PNL2::procTotalPNLAssetData(const Exchange& exchange) {
  auto f = std::bind(&PNL2::setTotalPNLAssetData, this, &exchange,
                     std::placeholders::_1);
  foreachPNLDataValue(this->m_data.mutable_total(), f);
}

// 处理ctrlnode里的统计数据
void PNL2::procCtrlNodeData(const Exchange& exchange) {
  //!!! 这样写法效率不高，asset多个的时候会遍历多次
  //!!! 还有个可能的问题，就是如果初始资金池就有一部分asset，价格可能会不准
  for (auto it = this->m_mapAssets.begin(); it != this->m_mapAssets.end();
       ++it) {
    auto t = this->m_data.mutable_total();
    double lastprice = 0;
    for (auto i = 0; i < t->lstctrl_size(); ++i) {
      auto cc = t->mutable_lstctrl(i);

      if (cc->type() == tradingpb::CTRL_BUY && cc->dst().code() == *it) {
        Money cost;
        Volume volume;
        this->getAssetInfo(exchange, cc->dst().code().c_str(), cc->ts(), cost,
                           volume);

        if (volume > 0) {
          lastprice = cost / volume;
          cc->set_averageholdingprice(lastprice);
        }
      } else if (cc->type() == tradingpb::CTRL_SELL) {
        CandleData cd;
        if (exchange.getDataWithTimestamp(it->c_str(), cc->ts(), cd)) {
          cc->set_sellprice(cd.close);
        }

        cc->set_averageholdingprice(lastprice);
      }
    }
  }
}

void PNL2::onBuildEnd(const Exchange& exchange) {
  auto t = this->m_data.mutable_total();

  LOG(INFO) << "onBuildEnd " << t->values_size();

  this->calcMaxDrawdown();
  this->calcMaxDrawup();

  this->calcTotalReturns();

  this->calcAnnualizedReturns(exchange);

  this->calcAnnualizedVolatility(exchange);

  this->calcSharpe(exchange);

  this->calcVariance();

  this->calcControls();
}

void PNL2::calcMaxDrawdown() {
  // 最大回撤算法，不应该取最低点来算
  // 从最尾部开始算，找到最尾部以前的最大值，再找到这个最大值以后的最小值，然后从最大值再向前重复计算

  auto t = this->m_data.mutable_total();

  int si = -1, ei = -1;
  float mdd = 0;
  int ci = t->values_size() - 1;

  while (ci > 0) {
    auto csi = this->findPreMax(ci);
    if (csi < 0) {
      break;
    }

    auto cei = this->findNextMin(csi);
    if (cei < 0) {
      break;
    }

    // auto cmdd = 0;
    // if (this->m_lst[csi].profitRatio == 0) {
    //   cmdd = fabs(this->m_lst[cei].profitRatio);
    // } else {
    auto cmdd = fabs(t->values(csi).pervalue() - t->values(cei).pervalue()) *
                1.0f / t->values(csi).pervalue();
    // }

    if (cmdd >= mdd) {
      si = csi;
      ei = cei;
      mdd = cmdd;
    }

    ci = this->findPreUpMin(csi);
  }

  t->set_maxdrawdown(mdd);
  t->set_maxdrawdownstartts(t->values(si).ts());
  t->set_maxdrawdownendts(t->values(ei).ts());
}

// 找到 starti 前面的最高点
int PNL2::findPreMax(int starti) {
  auto t = this->m_data.total();

  if (t.values_size() <= 0) {
    return -1;
  }

  if (starti < 0 || starti >= t.values_size()) {
    starti = t.values_size() - 1;
  }

  auto ci = starti;
  auto mm = t.values(starti).value();
  for (int i = starti - 1; i >= 0; --i) {
    if (mm <= t.values(i).value()) {
      ci = i;
      mm = t.values(i).value();
    }
  }

  return ci;
}

// 找到 starti 后面的最低点
int PNL2::findNextMin(int starti) {
  auto t = this->m_data.total();

  if (t.values_size() <= 0) {
    return -1;
  }

  if (starti < 0 || starti >= t.values_size()) {
    starti = 0;
  }

  auto ci = starti;
  auto mm = t.values(starti).value();
  for (int i = starti + 1; i < t.values_size(); ++i) {
    if (mm >= t.values(i).value()) {
      ci = i;
      mm = t.values(i).value();
    }
  }

  return ci;
}

// 找到starti前面第一个阶段性低点
// 假设starti是一个高点，该函数返回这个高点前一个下跌的终点
int PNL2::findPreUpMin(int starti) {
  auto t = this->m_data.total();

  if (t.values_size() <= 0) {
    return -1;
  }

  if (starti < 0 || starti >= t.values_size()) {
    starti = t.values_size() - 1;
  }

  auto mm = t.values(starti).value();

  for (int i = starti - 1; i >= 0; --i) {
    if (t.values(i).value() > t.values(i + 1).value()) {
      return i + 1;
    }
  }

  return 0;
}

void PNL2::calcMaxDrawup() {
  // 最大回撤算法，不应该取最低点来算
  // 从最尾部开始算，找到最尾部以前的最大值，再找到这个最大值以后的最小值，然后从最大值再向前重复计算

  auto t = this->m_data.mutable_total();

  int si = -1, ei = -1;
  float mdd = 0;
  int ci = t->values_size() - 1;

  while (ci > 0) {
    auto csi = this->findPreMin(ci);
    if (csi < 0) {
      break;
    }

    auto cei = this->findNextMax(csi);
    if (cei < 0) {
      break;
    }

    // auto cmdd = 0;
    // if (this->m_lst[csi].profitRatio == 0) {
    //   cmdd = fabs(this->m_lst[cei].profitRatio);
    // } else {
    auto cmdd = fabs(t->values(csi).pervalue() - t->values(cei).pervalue()) *
                1.0f / t->values(csi).pervalue();
    // }

    if (cmdd >= mdd) {
      si = csi;
      ei = cei;
      mdd = cmdd;
    }

    ci = this->findPreDownMax(csi);
  }

  t->set_maxdrawup(mdd);
  t->set_maxdrawupstartts(t->values(si).ts());
  t->set_maxdrawupendts(t->values(ei).ts());
}

// 找到 starti 前面的最低点
int PNL2::findPreMin(int starti) {
  auto t = this->m_data.total();

  if (t.values_size() <= 0) {
    return -1;
  }

  if (starti < 0 || starti >= t.values_size()) {
    starti = t.values_size() - 1;
  }

  auto ci = starti;
  auto mm = t.values(starti).value();
  for (int i = starti - 1; i >= 0; --i) {
    if (mm >= t.values(i).value()) {
      ci = i;
      mm = t.values(i).value();
    }
  }

  return ci;
}

// 找到 starti 后面的最高点
int PNL2::findNextMax(int starti) {
  auto t = this->m_data.total();

  if (t.values_size() <= 0) {
    return -1;
  }

  if (starti < 0 || starti >= t.values_size()) {
    starti = 0;
  }

  auto ci = starti;
  auto mm = t.values(starti).value();
  for (int i = starti + 1; i < t.values_size(); ++i) {
    if (mm <= t.values(i).value()) {
      ci = i;
      mm = t.values(i).value();
    }
  }

  return ci;
}

// 找到starti前面第一个阶段性高点
// 假设starti是一个低点，该函数返回这个低点前一个上涨的终点
int PNL2::findPreDownMax(int starti) {
  auto t = this->m_data.total();

  if (t.values_size() <= 0) {
    return -1;
  }

  if (starti < 0 || starti >= t.values_size()) {
    starti = t.values_size() - 1;
  }

  auto mm = t.values(starti).value();
  for (int i = starti - 1; i >= 0; --i) {
    if (t.values(i).value() < t.values(i + 1).value()) {
      return i + 1;
    }
  }

  return 0;
}

void PNL2::calcTotalReturns() {
  auto t = this->m_data.mutable_total();

  if (t->values_size() <= 0) {
    return;
  }

  t->set_totalreturns(t->values(t->values_size() - 1).pervalue());
}

void PNL2::calcSharpe(const Exchange& exchange) {
  // https://www.zhihu.com/question/27264526

  auto t = this->m_data.mutable_total();

  t->set_sharpe((t->annualizedreturns() - exchange.getRiskFreeInterestRate()) /
                t->annualizedvolatility());
}

void PNL2::calcAnnualizedReturns(const Exchange& exchange) {
  auto t = this->m_data.mutable_total();

  if (t->values_size() <= 0) {
    return;
  }

  t->set_annualizedreturns((t->values(t->values_size() - 1).pervalue() - 1) /
                           t->values_size() * exchange.getTradingDays4Year());
}

void PNL2::calcAnnualizedVolatility(const Exchange& exchange) {
  // https://www.zhihu.com/question/19770602
  // https://wiki.mbalib.com/wiki/%E5%8E%86%E5%8F%B2%E6%B3%A2%E5%8A%A8%E7%8E%87

  auto t = this->m_data.mutable_total();

  if (t->values_size() <= 0) {
    return;
  }

  float* pU = new float[t->values_size() - 1];

  for (int i = 1; i < t->values_size(); ++i) {
    pU[i - 1] = log(t->values(i).pervalue() / t->values(i - 1).pervalue());
  }

  float s = gsl_stats_float_sd(pU, 1, t->values_size() - 1);
  t->set_annualizedvolatility(s * sqrt(exchange.getTradingDays4Year()));

  delete[] pU;
}

void PNL2::calcVariance() {
  auto t = this->m_data.mutable_total();

  if (t->values_size() <= 0) {
    return;
  }

  float* pU = new float[t->values_size()];

  // float sm = t->values(0).pervalue();
  for (int i = 0; i < t->values_size(); ++i) {
    pU[i] = t->values(i).pervalue();
  }

  float s = gsl_stats_float_variance(pU, 1, t->values_size());
  t->set_variance(s);

  delete[] pU;
}

void PNL2::calcControls() {
  auto mt = this->m_data.mutable_total();
  mt->set_buytimes(0);
  mt->set_selltimes(0);
  mt->set_stoplosstimes(0);
  mt->set_wintimes(0);

  auto t = this->m_data.total();
  double lastprice = 0;
  for (auto i = 0; i < t.lstctrl_size(); ++i) {
    auto cc = t.lstctrl(i);

    if (cc.type() == tradingpb::CTRL_SELL) {
      mt->set_selltimes(mt->selltimes() + 1);

      if (cc.sellprice() > cc.averageholdingprice()) {
        mt->set_wintimes(mt->wintimes() + 1);
      }
    } else if (cc.type() == tradingpb::CTRL_BUY) {
      mt->set_buytimes(mt->buytimes() + 1);
    }
  }
}

CR2END