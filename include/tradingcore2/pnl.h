#ifndef __TRADINGCORE2_PNL_H__
#define __TRADINGCORE2_PNL_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/proto/tradingdb2.grpc.pb.h>

#include <map>
#include <string>
#include <vector>

CR2BEGIN

class PNL {
 public:
  struct Node {
    TimeStamp ts;
    Money invest;
    Money curMoney;
    float percentage;
    Money buy;
    Money sell;

    Node() : ts(0), invest(0), curMoney(0), percentage(0), buy(0), sell(0) {}
  };

  typedef std::vector<Node> List;

 public:
  PNL()
      : m_maxDrawdown(0),
        m_sharpe(0),
        m_annualizedReturns(0),
        m_annualizedVolatility(0),
        m_totalReturns(0),
        m_variance(0),
        m_maxDrawdownStartI(-1),
        m_maxDrawdownEndI(-1),
        m_perValidData(0),
        m_durationYear(0),
        m_maxUpDay(0),
        m_maxDownDay(0),
        m_maxUpWeek(0),
        m_maxDownWeek(0),
        m_maxUpMonth(0),
        m_maxDownMonth(0),
        m_maxUpYear(0),
        m_maxDownYear(0),
        m_maxMoneyUpDay(0),
        m_maxMoneyDownDay(0),
        m_maxMoneyUpWeek(0),
        m_maxMoneyDownWeek(0),
        m_maxMoneyUpMonth(0),
        m_maxMoneyDownMonth(0),
        m_maxMoneyUpYear(0),
        m_maxMoneyDownYear(0) {}
  ~PNL() { this->release(); }

 public:
  void release() { m_lst.clear(); }

  void initWithCandles(const tradingdb2pb::Candles& candles);

  void initInvest(const Exchange& exchange, Money invest, Money handMoney,
                  TimeStamp tsStart, TimeStamp tsEnd);

  int getLength() const { return m_lst.size(); }

  PNL::Node getNode(int index) const;

  void onBuildEnd(const Exchange& exchange);

 public:
  void onInitInvestTimeStamp(const Exchange& exchange, TimeStamp ts,
                             Money invest, Money handMoney);

  void pushData(TimeStamp ts, Money invest, Money curMoney);

  void chgData(TimeStamp ts, Money offInvest, Money offMoney);

  void setBuy(TimeStamp ts, Money money);

  void setSell(TimeStamp ts, Money money);

  void getTrainResult(TrainResult& tr);

 public:
  void print(const char* title);

  void saveCSV(const char* fn, bool useMoney);

 public:
  void calcMaxDrawdown();

  void calcValidDataPer(const tradingdb2pb::SymbolInfo& si,
                        const Exchange& exchange);

  void calcVariance();

  void calcMaxDate();

 protected:
  void calcMaxDate_Day();

  void calcMaxDate_Week();

  void calcMaxDate_Month();

  void calcMaxDate_Year();

  void calcSharpe(const Exchange& exchange);

  void calcAnnualizedReturns(const Exchange& exchange);

  void calcTotalReturns(const Exchange& exchange);

  void calcAnnualizedVolatility(const Exchange& exchange);

  // 找到 starti 前面的最高点
  int findPreMax(int starti);
  // 找到starti前面第一个阶段性低点
  // 假设starti是一个高点，该函数返回这个高点前一个下跌的终点
  int findPreUpMin(int starti);
  // 找到 starti 后面的最低点
  int findNextMin(int starti);

 public:
  List m_lst;

  float m_maxDrawdown;
  int m_maxDrawdownStartI;
  int m_maxDrawdownEndI;

  float m_sharpe;
  float m_annualizedReturns;
  float m_annualizedVolatility;
  float m_totalReturns;
  float m_variance;

  float m_perValidData;
  float m_durationYear;

  time_t m_maxUpDay;
  time_t m_maxDownDay;
  time_t m_maxUpWeek;
  time_t m_maxDownWeek;
  time_t m_maxUpMonth;
  time_t m_maxDownMonth;
  time_t m_maxUpYear;
  time_t m_maxDownYear;
  float m_maxMoneyUpDay;
  float m_maxMoneyDownDay;
  float m_maxMoneyUpWeek;
  float m_maxMoneyDownWeek;
  float m_maxMoneyUpMonth;
  float m_maxMoneyDownMonth;
  float m_maxMoneyUpYear;
  float m_maxMoneyDownYear;
};

CR2END

#endif  // __TRADINGCORE2_PNL_H__