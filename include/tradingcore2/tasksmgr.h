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
  TasksMgr() : m_maxTaskNums(0) { this->m_curTaskNums = 0; }
  ~TasksMgr() { this->release(); }

 protected:
  void release();

 public:
  void init(const Config& cfg);
  // runTradingTask - runTradingTask
  ::grpc::Status runTradingTask(const ::tradingpb::SimTradingParams* params,
                                ::tradingpb::PNLData* pnldata);

  // runTask - runTask
  ::grpc::Status runTask(const ::tradingpb::SimTradingParams* params,
                         ::tradingpb::PNLData* pnldata);

  // getCurTaskNums - get current tasks number
  int getCurTaskNums() { return this->m_curTaskNums.load(std::memory_order_relaxed); }

 protected:
  int m_maxTaskNums;
  std::atomic<int> m_curTaskNums;
};

CR2END

#endif  // __TRADINGCORE2_TASKSMGR_H__