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
  };

  typedef std::vector<Node> List;

 public:
  PNL() {}
  ~PNL() { this->release(); }

 public:
  bool buildData(const Wallet& wallet, TimeStamp startts, TimeStamp endts);

  void release() { m_lst.clear(); }

 public:
  void onHistoryNode(const WalletHistoryNode& node);

  void pushData(TimeStamp ts, Money invest, Money curMoney);

 protected:
  List m_lst;
};

CR2END

#endif  // __TRADINGCORE2_PNL_H__