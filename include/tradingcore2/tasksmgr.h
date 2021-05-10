#ifndef __TRADINGCORE2_TASKSMGR_H__
#define __TRADINGCORE2_TASKSMGR_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/config.h>
#include <tradingcore2/protos/tradingdb2.grpc.pb.h>

#include <atomic>
#include <functional>

CR2BEGIN

class TasksMgr {
 public:
  static TasksMgr* getSingleton();

 protected:
  TasksMgr() {}
  ~TasksMgr() { this->release(); }

 protected:
  void release();

 public:
  void init(const Config& cfg);
  // calcPNL - calcPNL
  ::grpc::Status _calcPNL(const ::tradingpb::SimTradingParams* params,
                          ::tradingpb::PNLData* pnldata);

 protected:
  int m_maxTaskNums;
  std::atomic<int> m_curTaskNums;
};

CR2END

#endif  // __TRADINGCORE2_TASKSMGR_H__