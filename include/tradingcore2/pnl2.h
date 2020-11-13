#ifndef __TRADINGCORE2_PNL2_H__
#define __TRADINGCORE2_PNL2_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/protos/tradingnode2.grpc.pb.h>

#include <map>
#include <string>
#include <vector>

CR2BEGIN

class PNL2 {
 public:
  struct Node {
    TimeStamp ts;
    Money invest;
    Money curMoney;
    float percentage;
    float profitRatio;
    Money buy;
    Money sell;

    Node()
        : ts(0),
          invest(0),
          curMoney(0),
          percentage(0),
          buy(0),
          sell(0),
          profitRatio(0) {}
  };

  typedef std::vector<Node> List;

 public:
  PNL2() {}
  ~PNL2() { this->release(); }

 public:
  void release() {}
 public:
  tradingpb::PNLData m_data;
};

CR2END

#endif  // __TRADINGCORE2_PNL2_H__