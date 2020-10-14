
#include <gsl/statistics/gsl_statistics_float.h>
#include <math.h>
#include <tradingcore2/csv.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/pnl.h>
#include <tradingcore2/wallet.h>

#include <functional>

CR2BEGIN

void PNL::initWithCandles(const tradingdb2pb::Candles& candles) {
  for (auto i = 0; i < candles.candles_size(); ++i) {
    auto cc = candles.candles(i);

    this->pushData(cc.ts(), cc.close(), cc.close());
  }
}

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

void PNL::setBuy(TimeStamp ts, Money money) {
  for (auto it = this->m_lst.begin(); it != this->m_lst.end(); ++it) {
    if (ts == it->ts) {
      it->buy += money;

      return;
    }
  }

  assert(false && "PNL::setBuy can't find timestamp.");
}

void PNL::setSell(TimeStamp ts, Money money) {
  for (auto it = this->m_lst.begin(); it != this->m_lst.end(); ++it) {
    if (ts == it->ts) {
      it->sell += money;

      return;
    }
  }

  assert(false && "PNL::setSell can't find timestamp.");
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
  if (this->m_lst.empty()) {
    return;
  }

  this->calcMaxDrawdown(exchange);

  this->calcTotalReturns(exchange);

  this->calcAnnualizedReturns(exchange);

  this->calcAnnualizedVolatility(exchange);

  this->calcSharpe(exchange);

  this->calcVariance(exchange);
}

void PNL::calcMaxDrawdown(const Exchange& exchange) {
  // 最大回撤算法，不应该取最低点来算
  // 从最尾部开始算，找到最尾部以前的最大值，只要当前节点在最大值后面，就可以省掉一个最大值遍历

  int maxindex = -1;
  float mdd = 0;
  Money curmaxmoney = -1;

  for (auto it = this->m_lst.rbegin(); it != this->m_lst.rend(); ++it) {
    auto co = this->m_lst.size() - 1 - std::distance(this->m_lst.rbegin(), it);
    if (maxindex == -1 || maxindex >= co) {
      curmaxmoney = -1;
      for (auto it2 = it + 1; it2 != this->m_lst.rend(); ++it2) {
        // 这里 >= ，表示取最远的一个值，可以节省掉更多可能的遍历
        if (it2->curMoney >= curmaxmoney) {
          curmaxmoney = it2->curMoney;
          maxindex =
              this->m_lst.size() - 1 - std::distance(this->m_lst.rbegin(), it2);
        }
      }
    }

    if (curmaxmoney > it->curMoney) {
      float cdd = (curmaxmoney - it->curMoney) / curmaxmoney;
      if (cdd > mdd) {
        mdd = cdd;
      }
    }
  }

  this->m_maxDrawdown = mdd;
}

void PNL::calcSharpe(const Exchange& exchange) {
  // https://www.zhihu.com/question/27264526

  this->m_sharpe =
      (this->m_annualizedReturns - exchange.getRiskFreeInterestRate()) /
      this->m_annualizedVolatility;
}

void PNL::calcAnnualizedReturns(const Exchange& exchange) {
  this->m_annualizedReturns =
      (this->m_lst.back().curMoney / this->m_lst.begin()->curMoney - 1) /
      this->m_lst.size() * exchange.getTradingDays4Year();
}

void PNL::calcTotalReturns(const Exchange& exchange) {
  this->m_totalReturns =
      (this->m_lst.back().curMoney / this->m_lst.begin()->curMoney - 1);
}

void PNL::calcAnnualizedVolatility(const Exchange& exchange) {
  // https://www.zhihu.com/question/19770602
  // https://wiki.mbalib.com/wiki/%E5%8E%86%E5%8F%B2%E6%B3%A2%E5%8A%A8%E7%8E%87

  float* pU = new float[this->m_lst.size() - 1];

  for (int i = 1; i < this->m_lst.size(); ++i) {
    pU[i - 1] = log(this->m_lst[i].curMoney / this->m_lst[i - 1].curMoney);
  }

  float s = gsl_stats_float_sd(pU, 1, this->m_lst.size() - 1);
  this->m_annualizedVolatility = s * sqrt(exchange.getTradingDays4Year());

  delete[] pU;
}

void PNL::print(const char* title) {
  printf("-= %s =-\n", title);
  printf("total returns: %.3f%%\n", this->m_totalReturns * 100);
  printf("max drawdown: %.3f%%\n", this->m_maxDrawdown * 100);
  printf("sharpe: %.5f\n", this->m_sharpe);
  printf("annualized returns: %.3f%%\n", this->m_annualizedReturns * 100);
  printf("annualized volatility: %.3f%%\n", this->m_annualizedVolatility * 100);
}

void PNL::saveCSV(const char* fn, bool useMoney) {
  if (useMoney) {
    auto onhead = [](FILE* fp) { fprintf(fp, "ts,invest,money,buy,sell\r\n"); };

    List& lst = this->m_lst;
    auto onrow = [&lst](FILE* fp, int row) {
      if (lst.size() <= row) {
        return false;
      }

      fprintf(fp, "%ld,%.3f,%.3f,%.3f,%.3f\r\n", lst[row].ts, lst[row].invest,
              lst[row].curMoney, lst[row].buy, lst[row].sell);

      return true;
    };

    tr2::saveCSV(fn, onhead, onrow);
  } else {
    auto onhead = [](FILE* fp) {
      fprintf(fp, "ts,invest,percentage,buy,sell\r\n");
    };

    List& lst = this->m_lst;
    auto onrow = [&lst](FILE* fp, int row) {
      if (lst.size() <= row) {
        return false;
      }

      fprintf(fp, "%ld,%.3f,%.2f,%.3f,%.3f\r\n", lst[row].ts, lst[row].invest,
              lst[row].percentage * 100, lst[row].buy, lst[row].sell);

      return true;
    };

    tr2::saveCSV(fn, onhead, onrow);
  }
}

void PNL::calcVariance(const Exchange& exchange) {
  float* pU = new float[this->m_lst.size()];

  for (int i = 0; i < this->m_lst.size(); ++i) {
    pU[i] = this->m_lst[i].curMoney;
  }

  float s = gsl_stats_float_variance(pU, 1, this->m_lst.size());
  this->m_variance = s;

  delete[] pU;
}

void PNL::getTrainResult(TrainResult& tr) {
  tr.totalReturn = this->m_totalReturns;
  tr.maxDrawDown = this->m_maxDrawdown;
  tr.sharpe = this->m_sharpe;
  tr.annualizedReturns = this->m_annualizedReturns;
  tr.annualizedVolatility = this->m_annualizedVolatility;
  tr.variance = this->m_variance;
}

CR2END