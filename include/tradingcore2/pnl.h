#ifndef __TRADINGCORE2_PNL_H__
#define __TRADINGCORE2_PNL_H__

#include <tradingcore2/basedef.h>

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

    Node() : ts(0), invest(0), curMoney(0), percentage(0) {}
  };

  typedef std::vector<Node> List;

 public:
  PNL()
      : m_maxDrawdown(0),
        m_sharpe(0),
        m_annualizedReturns(0),
        m_annualizedVolatility(0) {}
  ~PNL() { this->release(); }

 public:
  void release() { m_lst.clear(); }

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

 protected:
  void calcMaxDrawdown(const Exchange& exchange);

  void calcSharpe(const Exchange& exchange);

  void calcAnnualizedReturns(const Exchange& exchange);

  void calcAnnualizedVolatility(const Exchange& exchange);

 protected:
  List m_lst;
  float m_maxDrawdown;
  float m_sharpe;
  float m_annualizedReturns;
  float m_annualizedVolatility;
};

CR2END

#endif  // __TRADINGCORE2_PNL_H__