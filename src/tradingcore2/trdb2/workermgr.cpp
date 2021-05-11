#include <tradingcore2/protos/tradingdb2.grpc.pb.h>
#include <tradingcore2/trdb2/workermgr.h>
#include <tradingcore2/utils.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

CR2BEGIN

void WorkerMgr::release() {
  std::lock_guard<std::mutex> lock(this->m_mtx);

  for (_Map::iterator it = this->m_map.begin(); it != this->m_map.end(); ++it) {
    delete it->second.pThread;
  }

  m_map.clear();
}

bool WorkerMgr::insWorker(int workerID, std::thread *pThread) {
  std::lock_guard<std::mutex> lock(this->m_mtx);

  _Pair p;
  p.first = workerID;
  p.second.workerID = workerID;
  p.second.pThread = pThread;
  p.second.isEnd = false;

  auto ret = this->m_map.insert(p);
  return ret.second;
}

void WorkerMgr::delWorker(int workerID) {
  std::lock_guard<std::mutex> lock(this->m_mtx);

  auto ts = std::time(NULL);
  if (ts - this->m_lastDeleteTs >= 60) {
    this->m_lastDeleteTs = ts;

    for (auto it = this->m_map.begin(); it != this->m_map.end(); ++it) {
      if (it->second.isEnd && !it->second.pThread->joinable()) {
        delete it->second.pThread;
        this->m_map.erase(it);
        break;
      }
    }
  }

  auto it = this->m_map.find(workerID);
  if (it != this->m_map.end()) {
    it->second.isEnd = true;
    // delete it->second.pThread;
    // this->m_map.erase(it);
  }
}

int WorkerMgr::newWorkerID() {
  std::lock_guard<std::mutex> lock(this->m_mtx);
  return ++latestWorkerID;
}

CR2END