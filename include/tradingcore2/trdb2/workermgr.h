#ifndef __TRADINGCORE2_TRDB2_WORKMGR_H__
#define __TRADINGCORE2_TRDB2_WORKMGR_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/protos/tradingdb2.grpc.pb.h>

#include <functional>
#include <mutex>
#include <thread>

CR2BEGIN

class WorkerMgr {
 public:
  struct Worker {
    int workerID;
    std::thread *pThread;
    bool isEnd;
  };

  typedef std::map<int, Worker> _Map;
  typedef std::pair<int, Worker> _Pair;

 public:
  WorkerMgr() : latestWorkerID(0), m_lastDeleteTs(0) {}
  ~WorkerMgr() { this->release(); }

 public:
  void release();

 public:
  int newWorkerID();

  bool insWorker(int workerID, std::thread *pThread);

  void delWorker(int workerID);

 private:
  _Map m_map;
  int latestWorkerID;
  std::mutex m_mtx;
  std::time_t m_lastDeleteTs;
};

CR2END

#endif  // __TRADINGCORE2_TRDB2_WORKMGR_H__