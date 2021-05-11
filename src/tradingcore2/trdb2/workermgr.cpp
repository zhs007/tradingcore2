#include <tradingcore2/protos/tradingdb2.grpc.pb.h>
#include <tradingcore2/trdb2/workermgr.h>
#include <tradingcore2/utils.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

CR2BEGIN

void WorkerMgr::release() {
  for (_Map::iterator it = this->m_map.begin(); it != this->m_map.end(); ++it) {
    delete it->second.pThread;
  }

  m_map.clear();
}

bool WorkerMgr::insWorker(int workerID, std::thread *pThread) {
  _Pair p;
  p.first = workerID;
  p.second.workerID = workerID;
  p.second.pThread = pThread;

  auto ret = this->m_map.insert(p);
  return ret.first;
}

void WorkerMgr::delWorker(int workerID) {
  auto it = this->m_map.find(workerID);
  if (it != this->m_map.end()) {
    delete it->second.pThread;
    this->m_map.erase(it);
  }
}

CR2END