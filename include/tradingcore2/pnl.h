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
  PNL() {}
  ~PNL() { this->release(); }

 public:
  bool buildData(const Wallet& wallet, TimeStamp startts, TimeStamp endts);

  void release() { m_lst.clear(); }

  void initInvest(const Exchange& exchange, Money invest, Money handMoney,
                  TimeStamp tsStart, TimeStamp tsEnd);

  int getLength() const { return m_lst.size(); }

  PNL::Node getNode(int index) const;

 public:
  void onHistoryNode(const WalletHistoryNode& node);

  void onInitInvestTimeStamp(const Exchange& exchange, TimeStamp ts,
                             Money invest, Money handMoney);

  void pushData(TimeStamp ts, Money invest, Money curMoney);

  void chgData(TimeStamp ts, Money offInvest, Money offMoney);

 protected:
  List m_lst;
};

CR2END

#endif  // __TRADINGCORE2_PNL_H__