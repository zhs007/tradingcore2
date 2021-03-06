
#include <gsl/statistics/gsl_statistics_float.h>
#include <math.h>
#include <tradingcore2/csv.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/pnl.h>
#include <tradingcore2/utils.h>
#include <tradingcore2/wallet.h>

#include <functional>

CR2BEGIN

void PNL::initWithCandles(const tradingpb::Candles& candles) {
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

  if (this->m_lst.empty()) {
    n.profitRatio = 1;
  } else {
    auto it = this->m_lst.begin();
    n.profitRatio = curMoney * 1.0 / it->curMoney;
  }

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

  this->calcMaxDrawdown();

  this->calcMaxDrawup();

  this->calcTotalReturns(exchange);

  this->calcAnnualizedReturns(exchange);

  this->calcAnnualizedVolatility(exchange);

  this->calcSharpe(exchange);

  this->calcVariance();

  this->calcMaxDate();
}

// 找到 starti 前面的最高点
int PNL::findPreMax(int starti) {
  if (this->m_lst.empty()) {
    return -1;
  }

  if (starti < 0 || starti >= this->m_lst.size()) {
    starti = this->m_lst.size() - 1;
  }

  auto ci = starti;
  auto mm = this->m_lst[starti].curMoney;
  for (int i = starti - 1; i >= 0; --i) {
    if (mm <= this->m_lst[i].curMoney) {
      ci = i;
      mm = this->m_lst[i].curMoney;
    }
  }

  return ci;
}

// 找到 starti 后面的最低点
int PNL::findNextMin(int starti) {
  if (this->m_lst.empty()) {
    return -1;
  }

  if (starti < 0 || starti >= this->m_lst.size()) {
    starti = 0;
  }

  auto ci = starti;
  auto mm = this->m_lst[starti].curMoney;
  for (int i = starti + 1; i < this->m_lst.size(); ++i) {
    if (mm >= this->m_lst[i].curMoney) {
      ci = i;
      mm = this->m_lst[i].curMoney;
    }
  }

  return ci;
}

// 找到starti前面第一个阶段性低点
// 假设starti是一个高点，该函数返回这个高点前一个下跌的终点
int PNL::findPreUpMin(int starti) {
  if (this->m_lst.empty()) {
    return -1;
  }

  if (starti < 0 || starti >= this->m_lst.size()) {
    starti = this->m_lst.size() - 1;
  }

  auto mm = this->m_lst[starti].curMoney;
  for (int i = starti - 1; i >= 0; --i) {
    if (this->m_lst[i].curMoney > this->m_lst[i + 1].curMoney) {
      return i + 1;
    }
  }

  return 0;
}

// 找到 starti 前面的最低点
int PNL::findPreMin(int starti) {
  if (this->m_lst.empty()) {
    return -1;
  }

  if (starti < 0 || starti >= this->m_lst.size()) {
    starti = this->m_lst.size() - 1;
  }

  auto ci = starti;
  auto mm = this->m_lst[starti].curMoney;
  for (int i = starti - 1; i >= 0; --i) {
    if (mm >= this->m_lst[i].curMoney) {
      ci = i;
      mm = this->m_lst[i].curMoney;
    }
  }

  return ci;
}

// 找到starti前面第一个阶段性高点
// 假设starti是一个低点，该函数返回这个低点前一个上涨的终点
int PNL::findPreDownMax(int starti) {
  if (this->m_lst.empty()) {
    return -1;
  }

  if (starti < 0 || starti >= this->m_lst.size()) {
    starti = this->m_lst.size() - 1;
  }

  auto mm = this->m_lst[starti].curMoney;
  for (int i = starti - 1; i >= 0; --i) {
    if (this->m_lst[i].curMoney < this->m_lst[i + 1].curMoney) {
      return i + 1;
    }
  }

  return 0;
}

void PNL::calcMaxDrawdown() {
  // 最大回撤算法，不应该取最低点来算
  // 从最尾部开始算，找到最尾部以前的最大值，再找到这个最大值以后的最小值，然后从最大值再向前重复计算

  int si = -1, ei = -1;
  float mdd = 0;
  int ci = this->m_lst.size() - 1;

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
    auto cmdd =
        fabs(this->m_lst[csi].profitRatio - this->m_lst[cei].profitRatio) *
        1.0f / this->m_lst[csi].profitRatio;
    // }

    if (cmdd > mdd) {
      si = csi;
      ei = cei;
      mdd = cmdd;
    }

    ci = this->findPreUpMin(csi);
  }

  this->m_maxDrawdown = mdd;
  this->m_maxDrawdownStartI = si;
  this->m_maxDrawdownEndI = ei;
}

// 找到 starti 后面的最高点
int PNL::findNextMax(int starti) {
  if (this->m_lst.empty()) {
    return -1;
  }

  if (starti < 0 || starti >= this->m_lst.size()) {
    starti = 0;
  }

  auto ci = starti;
  auto mm = this->m_lst[starti].curMoney;
  for (int i = starti + 1; i < this->m_lst.size(); ++i) {
    if (mm <= this->m_lst[i].curMoney) {
      ci = i;
      mm = this->m_lst[i].curMoney;
    }
  }

  return ci;
}

void PNL::calcMaxDrawup() {
  // 最大回撤算法，不应该取最低点来算
  // 从最尾部开始算，找到最尾部以前的最大值，再找到这个最大值以后的最小值，然后从最大值再向前重复计算

  int si = -1, ei = -1;
  float mdd = 0;
  int ci = this->m_lst.size() - 1;

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
    auto cmdd =
        fabs(this->m_lst[csi].profitRatio - this->m_lst[cei].profitRatio) *
        1.0f / this->m_lst[csi].profitRatio;
    // }

    if (cmdd > mdd) {
      si = csi;
      ei = cei;
      mdd = cmdd;
    }

    ci = this->findPreDownMax(csi);
  }

  this->m_maxDrawup = mdd;
  this->m_maxDrawupStartI = si;
  this->m_maxDrawupEndI = ei;
}

void PNL::calcSharpe(const Exchange& exchange) {
  // https://www.zhihu.com/question/27264526

  this->m_sharpe =
      (this->m_annualizedReturns - exchange.getRiskFreeInterestRate()) /
      this->m_annualizedVolatility;
}

void PNL::calcAnnualizedReturns(const Exchange& exchange) {
  // this->m_annualizedReturns =
  //     (this->m_lst.back().curMoney / this->m_lst.begin()->curMoney - 1) /
  //     this->m_lst.size() * exchange.getTradingDays4Year();

  time_t st = this->m_lst[0].ts;
  tm startti;
  timestamp2timeUTC(st, startti);

  time_t et = this->m_lst[this->m_lst.size() - 1].ts;
  tm endti;
  timestamp2timeUTC(et, endti);

  int yearoff = endti.tm_year - startti.tm_year;
  int dayoff = yearoff * 365 - int(startti.tm_yday) + int(endti.tm_yday);

  auto durationYear = dayoff * 1.0f / 365;

  if (durationYear < 1) {
    this->m_annualizedReturns =
        (this->m_lst.back().curMoney / this->m_lst.begin()->curMoney - 1);
  } else {
    this->m_annualizedReturns =
        (this->m_lst.back().curMoney / this->m_lst.begin()->curMoney - 1) /
        durationYear;
  }
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

void PNL::calcVariance() {
  float* pU = new float[this->m_lst.size()];

  float sm = this->m_lst[0].curMoney;
  for (int i = 0; i < this->m_lst.size(); ++i) {
    pU[i] = this->m_lst[i].curMoney / sm;
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

void PNL::calcValidDataPer(const tradingpb::SymbolInfo& si,
                           const Exchange& exchange) {
  this->m_perValidData = 0;
  this->m_durationYear = 0;

  if (this->m_lst.empty()) {
    return;
  }

  if (si.has_fund()) {
    time_t st = si.fund().createtime();
    tm startti;
    timestamp2timeUTC(st, startti);

    time_t et = this->m_lst[this->m_lst.size() - 1].ts;
    tm endti;
    timestamp2timeUTC(et, endti);

    int yearoff = endti.tm_year - startti.tm_year;
    int dayoff = yearoff * exchange.getTradingDays4Year() -
                 int(startti.tm_yday) + int(endti.tm_yday);

    this->m_perValidData = this->m_lst.size() * 1.0f / dayoff;
    this->m_durationYear = dayoff * 1.0f / 365;
  }
}

void PNL::calcMaxDate_Day() {
  this->m_maxUpDay = 0;
  this->m_maxDownDay = 0;
  this->m_maxMoneyUpDay = 0;
  this->m_maxMoneyDownDay = 0;
  this->m_offSDUpDay = 0;
  this->m_offSDDownDay = 0;
  this->m_sdDay = 0;

  if (this->m_lst.empty()) {
    return;
  }

  this->m_maxUpDay = this->m_lst[0].ts;
  this->m_maxDownDay = this->m_lst[0].ts;

  this->m_maxMoneyUpDay = -999999;
  this->m_maxMoneyDownDay = 999999;

  for (int i = 1; i < this->m_lst.size(); ++i) {
    auto co = (this->m_lst[i].curMoney - this->m_lst[i - 1].curMoney) /
              this->m_lst[i - 1].curMoney;

    if (this->m_maxMoneyUpDay < co) {
      this->m_maxMoneyUpDay = co;
      this->m_maxUpDay = this->m_lst[i].ts;
      // this->m_offSDUpDay = (this->m_maxMoneyUpDay - sd) / sd;
    }

    if (this->m_maxMoneyDownDay > co) {
      this->m_maxMoneyDownDay = co;
      this->m_maxDownDay = this->m_lst[i].ts;
      // this->m_offSDDownDay = (this->m_maxMoneyDownDay - sd) / sd;
    }
  }

  float sd, mean;
  this->calcDaySD(this->m_maxUpDay, sd, mean, true);
  this->m_sdDay = sd;
  this->m_meanDay = mean;
  this->m_offSDUpDay = (this->m_maxMoneyUpDay - mean) / sd;

  this->calcDaySD(this->m_maxDownDay, sd, mean, false);
  // this->m_sdDay = sd;
  this->m_offSDDownDay = (this->m_maxMoneyDownDay - mean) / sd;
}

void PNL::calcMaxDate_Week() {
  this->m_maxUpWeek = 0;
  this->m_maxDownWeek = 0;
  this->m_maxMoneyUpWeek = 0;
  this->m_maxMoneyDownWeek = 0;
  this->m_offSDUpWeek = 0;
  this->m_offSDDownWeek = 0;
  this->m_sdWeek = 0;

  if (this->m_lst.empty()) {
    return;
  }

  this->m_maxMoneyUpWeek = -999999;
  this->m_maxMoneyDownWeek = 999999;

  auto cm = getYearWeek(this->m_lst[0].ts);
  this->m_maxUpWeek = this->m_lst[0].ts;
  this->m_maxDownWeek = this->m_lst[0].ts;

  auto sst = this->m_lst[0].ts;
  auto msm = this->m_lst[0].curMoney;

  for (int i = 1; i < this->m_lst.size(); ++i) {
    auto ccm = getYearWeek(this->m_lst[i].ts);
    if (ccm != cm) {
      auto mo = (this->m_lst[i - 1].curMoney - msm) / msm;

      if (this->m_maxMoneyUpWeek < mo) {
        this->m_maxMoneyUpWeek = mo;
        this->m_maxUpWeek = sst;
        // this->m_offSDUpWeek = (this->m_maxMoneyUpWeek - sd) / sd;
      }

      if (this->m_maxMoneyDownWeek > mo) {
        this->m_maxMoneyDownWeek = mo;
        this->m_maxDownWeek = sst;
        // this->m_offSDDownWeek = (this->m_maxMoneyDownWeek - sd) / sd;
      }

      sst = this->m_lst[i].ts;
      msm = this->m_lst[i].curMoney;

      cm = ccm;
    }
  }

  float sd = this->calcWeekSD(this->m_maxUpWeek);
  this->m_sdWeek = sd;
  this->m_offSDUpWeek = (this->m_maxMoneyUpWeek - sd) / sd;

  sd = this->calcWeekSD(this->m_maxDownWeek);
  // this->m_sdWeek = sd;
  this->m_offSDDownWeek = (this->m_maxMoneyDownWeek - sd) / sd;
}

void PNL::calcMaxDate_Month() {
  this->m_maxUpMonth = 0;
  this->m_maxDownMonth = 0;
  this->m_maxMoneyUpMonth = 0;
  this->m_maxMoneyDownMonth = 0;
  this->m_offSDUpMonth = 0;
  this->m_offSDDownMonth = 0;
  this->m_sdMonth = 0;

  if (this->m_lst.empty()) {
    return;
  }

  float sd = this->calcMonthSD();
  this->m_sdMonth = sd;

  this->m_maxMoneyUpMonth = -999999;
  this->m_maxMoneyDownMonth = 999999;

  auto cm = getYearMonth(this->m_lst[0].ts);
  this->m_maxUpMonth = this->m_lst[0].ts;
  this->m_maxDownMonth = this->m_lst[0].ts;

  auto sst = this->m_lst[0].ts;
  auto msm = this->m_lst[0].curMoney;

  for (int i = 1; i < this->m_lst.size(); ++i) {
    auto ccm = getYearMonth(this->m_lst[i].ts);
    if (ccm != cm) {
      auto mo = (this->m_lst[i - 1].curMoney - msm) / msm;

      if (this->m_maxMoneyUpMonth < mo) {
        this->m_maxMoneyUpMonth = mo;
        this->m_maxUpMonth = sst;
        this->m_offSDUpMonth = (this->m_maxMoneyUpMonth - sd) / sd;
      }

      if (this->m_maxMoneyDownMonth > mo) {
        this->m_maxMoneyDownMonth = mo;
        this->m_maxDownMonth = sst;
        this->m_offSDDownMonth = (this->m_maxMoneyDownMonth - sd) / sd;
      }

      sst = this->m_lst[i].ts;
      msm = this->m_lst[i].curMoney;

      cm = ccm;
    }
  }
}

void PNL::calcMaxDate_Year() {
  this->m_maxUpYear = 0;
  this->m_maxDownYear = 0;
  this->m_maxMoneyUpYear = 0;
  this->m_maxMoneyDownYear = 0;
  this->m_offSDUpYear = 0;
  this->m_offSDDownYear = 0;
  this->m_sdYear = 0;

  if (this->m_lst.empty()) {
    return;
  }

  float sd = this->calcYearSD();
  this->m_sdYear = sd;

  this->m_maxMoneyUpYear = -999999;
  this->m_maxMoneyDownYear = 999999;

  auto cm = getYear(this->m_lst[0].ts);
  this->m_maxUpYear = this->m_lst[0].ts;
  this->m_maxDownYear = this->m_lst[0].ts;

  auto sst = this->m_lst[0].ts;
  auto msm = this->m_lst[0].curMoney;

  for (int i = 1; i < this->m_lst.size(); ++i) {
    auto ccm = getYear(this->m_lst[i].ts);
    if (ccm != cm) {
      auto mo = (this->m_lst[i - 1].curMoney - msm) / msm;

      if (this->m_maxMoneyUpYear < mo) {
        this->m_maxMoneyUpYear = mo;
        this->m_maxUpYear = sst;
        this->m_offSDUpYear = (this->m_maxMoneyUpYear - sd) / sd;
      }

      if (this->m_maxMoneyDownYear > mo) {
        this->m_maxMoneyDownYear = mo;
        this->m_maxDownYear = sst;
        this->m_offSDDownYear = (this->m_maxMoneyDownYear - sd) / sd;
      }

      sst = this->m_lst[i].ts;
      msm = this->m_lst[i].curMoney;

      cm = ccm;
    }
  }
}

void PNL::calcMaxDate() {
  this->calcMaxDate_Day();
  this->calcMaxDate_Week();
  this->calcMaxDate_Month();
  this->calcMaxDate_Year();
}

TimeStamp PNL::getMaxDrawdownStartTime() {
  if (this->m_maxDrawdownStartI >= 0 &&
      this->m_maxDrawdownStartI < m_lst.size()) {
    return this->m_lst[this->m_maxDrawdownStartI].ts;
  }

  return 0;
}

TimeStamp PNL::getMaxDrawdownEndTime() {
  if (this->m_maxDrawdownEndI >= 0 && this->m_maxDrawdownEndI < m_lst.size()) {
    return this->m_lst[this->m_maxDrawdownEndI].ts;
  }

  return 0;
}

TimeStamp PNL::getMaxDrawupStartTime() {
  if (this->m_maxDrawupStartI >= 0 && this->m_maxDrawupStartI < m_lst.size()) {
    return this->m_lst[this->m_maxDrawupStartI].ts;
  }

  return 0;
}

TimeStamp PNL::getMaxDrawupEndTime() {
  if (this->m_maxDrawupEndI >= 0 && this->m_maxDrawupEndI < m_lst.size()) {
    return this->m_lst[this->m_maxDrawupEndI].ts;
  }

  return 0;
}

void PNL::calcDaySD(time_t ts, float& sd, float& mean, bool bUp) {
  float* pU = new float[this->m_lst.size()];

  int nums = 0;
  float sm = this->m_lst[0].curMoney;
  for (int i = 0; i < this->m_lst.size(); ++i) {
    if ((bUp && this->m_lst[i].curMoney > sm) ||
        (!bUp && this->m_lst[i].curMoney < sm)) {
      pU[nums++] = (this->m_lst[i].curMoney - sm) / sm;
    }
    // if (this->m_lst[i].ts != ts) {
    // pU[nums++] = (this->m_lst[i].curMoney - sm) / sm;
    // }

    sm = this->m_lst[i].curMoney;
  }

  sd = gsl_stats_float_sd(pU, 1, nums);
  mean = gsl_stats_float_mean(pU, 1, nums);

  delete[] pU;

  // return sd;
}

float PNL::calcWeekSD(time_t ts) {
  float* pU = new float[this->m_lst.size()];
  int weeks = 0;
  auto myw = getYearWeekEx(ts);

  auto yw = getYearWeekEx(this->m_lst[0].ts);
  float sm = this->m_lst[0].curMoney;
  float em = this->m_lst[0].curMoney;
  for (int i = 1; i < this->m_lst.size(); ++i) {
    auto cyw = getYearWeekEx(this->m_lst[i].ts);
    if (cyw != yw) {
      if (cyw != myw) {
        pU[weeks++] = (em - sm) / sm;
      }

      yw = cyw;
      sm = this->m_lst[i].curMoney;
      em = this->m_lst[i].curMoney;
    } else {
      em = this->m_lst[i].curMoney;
    }
  }

  float sd = gsl_stats_float_sd(pU, 1, weeks);

  delete[] pU;

  return sd;
}

float PNL::calcMonthSD() {
  float* pU = new float[this->m_lst.size()];
  int months = 0;

  auto ym = getYearMonthEx(this->m_lst[0].ts);
  float sm = this->m_lst[0].curMoney;
  float em = this->m_lst[0].curMoney;
  for (int i = 1; i < this->m_lst.size(); ++i) {
    auto cym = getYearMonthEx(this->m_lst[i].ts);
    if (cym != ym) {
      pU[months] = (em - sm) / sm;
      months++;

      ym = cym;
      sm = this->m_lst[i].curMoney;
      em = this->m_lst[i].curMoney;
    } else {
      em = this->m_lst[i].curMoney;
    }
  }

  float sd = gsl_stats_float_sd(pU, 1, months);

  delete[] pU;

  return sd;
}

float PNL::calcYearSD() {
  float* pU = new float[this->m_lst.size()];
  int years = 0;

  auto y = getYear(this->m_lst[0].ts);
  float sm = this->m_lst[0].curMoney;
  float em = this->m_lst[0].curMoney;
  for (int i = 1; i < this->m_lst.size(); ++i) {
    auto cy = getYear(this->m_lst[i].ts);
    if (cy != y) {
      pU[years] = (em - sm) / sm;
      years++;

      y = cy;
      sm = this->m_lst[i].curMoney;
      em = this->m_lst[i].curMoney;
    } else {
      em = this->m_lst[i].curMoney;
    }
  }

  float sd = gsl_stats_float_sd(pU, 1, years);

  delete[] pU;

  return sd;
}

CR2END