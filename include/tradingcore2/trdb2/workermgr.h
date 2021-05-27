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
  WorkerMgr(const Config &cfg)
      : m_latestWorkerID(0),
        m_lastDeleteTs(0),
        m_curFinishedTasks(0),
        m_finishedTasks(0) {
    init(cfg);
  }
  ~WorkerMgr() { this->release(); }

 public:
  void release();

 public:
  int newWorkerID();

  bool insWorker(int workerID, std::thread *pThread);

  void delWorker(int workerID);

  int countRunningWorkerNums();

  bool hasFreeWorker();

  bool hasRunningWorker();

  int getMaxWorkerNums() const { return m_maxWorkerNums; }

  bool canFinish();

 protected:
  void init(const Config &cfg);

 private:
  // const Config* m_pCfg;
  _Map m_map;
  int m_latestWorkerID;
  std::mutex m_mtx;
  std::time_t m_lastDeleteTs;
  int m_maxWorkerNums;
  int m_curFinishedTasks;
  int m_finishedTasks;
};

CR2END

#endif  // __TRADINGCORE2_TRDB2_WORKMGR_H__