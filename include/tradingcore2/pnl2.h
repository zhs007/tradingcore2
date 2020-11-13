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
  PNL2() {}
  ~PNL2() { this->release(); }

 public:
  void release();

 public:
  void initInvest(const Exchange& exchange, Money invest, Money handMoney,
                  TimeStamp tsStart, TimeStamp tsEnd);

  void onInitInvestTimeStamp(const Exchange& exchange, TimeStamp ts,
                             Money invest, Money handMoney);

 public:
  tradingpb::PNLData m_data;
};

CR2END

#endif  // __TRADINGCORE2_PNL2_H__