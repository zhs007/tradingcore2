#include <tradingcore2/config.h>
#include <tradingcore2/protos/tradingdb2.grpc.pb.h>
#include <tradingcore2/trdb2/workermgr.h>
#include <tradingcore2/utils.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

CR2BEGIN

void WorkerMgr::init(const Config& cfg) {
  long cpus = sysconf(_SC_NPROCESSORS_ONLN);  // get # of online cores
  if (cfg.taskNums == 0) {
    m_maxWorkerNums = cpus;
  } else if (cfg.taskNums >= cpus) {
    m_maxWorkerNums = cpus;
  } else if (cfg.taskNums > 0) {
    m_maxWorkerNums = cfg.taskNums;
  } else if (cfg.taskNums < 0) {
    if (-cfg.taskNums < cpus) {
      m_maxWorkerNums = cpus + cfg.taskNums;
    } else {
      m_maxWorkerNums = 1;
    }
  } else {
    assert(false && "WorkerMgr::init() error");
  }

  LOG(INFO) << "max workers num :" << m_maxWorkerNums;
}

void WorkerMgr::release() {
  std::lock_guard<std::mutex> lock(this->m_mtx);

  for (auto it = this->m_map.begin(); it != this->m_map.end();) {
    if (it->second.isEnd && !it->second.pThread->joinable()) {
      delete it->second.pThread;
      it = this->m_map.erase(it);
    } else {
      ++it;
    }
  }

  LOG(INFO) << "WorkerMgr::release non-free " << m_map.size();

  // for (_Map::iterator it = this->m_map.begin(); it != this->m_map.end();
  // ++it) {
  //   delete it->second.pThread;
  // }

  // m_map.clear();
}

bool WorkerMgr::insWorker(int workerID, std::thread* pThread) {
  std::lock_guard<std::mutex> lock(this->m_mtx);

  _Pair p;
  p.first = workerID;
  p.second.workerID = workerID;
  p.second.pThread = pThread;
  p.second.isEnd = false;

  auto ret = this->m_map.insert(p);

  LOG(INFO) << "WorkerMgr::insWorker " << workerID << " " << ret.second;

  return ret.second;
}

void WorkerMgr::delWorker(int workerID) {
  std::lock_guard<std::mutex> lock(this->m_mtx);

  auto ts = std::time(NULL);
  if (ts - this->m_lastDeleteTs >= 60) {
    this->m_lastDeleteTs = ts;

    for (auto it = this->m_map.begin(); it != this->m_map.end();) {
      if (it->second.isEnd && !it->second.pThread->joinable()) {
        LOG(INFO) << "WorkerMgr::delWorker " << workerID << " delete thread "
                  << it->first;

        delete it->second.pThread;
        it = this->m_map.erase(it);
      } else {
        ++it;
      }
    }
  }

  auto it = this->m_map.find(workerID);
  if (it != this->m_map.end()) {
    it->second.isEnd = true;

    LOG(INFO) << "WorkerMgr::delWorker " << workerID;
    // delete it->second.pThread;
    // this->m_map.erase(it);
  }
}

int WorkerMgr::newWorkerID() {
  std::lock_guard<std::mutex> lock(this->m_mtx);
  return ++m_latestWorkerID;
}

int WorkerMgr::countRunningWorkerNums() {
  auto nums = 0;
  for (auto it = this->m_map.begin(); it != this->m_map.end(); ++it) {
    if (!it->second.isEnd) {
      nums++;
    }
  }

  return nums;
}

bool WorkerMgr::hasFreeWorker() {
  std::lock_guard<std::mutex> lock(this->m_mtx);

  return this->countRunningWorkerNums() < this->m_maxWorkerNums;
}

bool WorkerMgr::hasRunningWorker() {
  std::lock_guard<std::mutex> lock(this->m_mtx);

  return this->countRunningWorkerNums() > 0;
}

CR2END