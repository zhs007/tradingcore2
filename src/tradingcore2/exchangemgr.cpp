#include <math.h>
#include <tradingcore2/exchangemgr.h>

#include <functional>

CR2BEGIN

ExchangeMgr* ExchangeMgr::getSingleton() {
  // for c++ 11
  static ExchangeMgr s_mgr;

  return &s_mgr;
}

Exchange* ExchangeMgr::newExchange(const char* name, const Config& cfg) {
  auto it = this->m_mapNewFunc.find(name);
  assert(it != this->m_mapNewFunc.end());

  auto pExchange = it->second(cfg);

  LOG(INFO) << "ExchangeMgr::newExchange " << name;

  return pExchange;
}

void ExchangeMgr::init(const Config& cfg) { this->m_pCfg = &cfg; }

void ExchangeMgr::regNewExchange(const char* name, FuncNewExchange func) {
  std::lock_guard<std::mutex> lock(this->m_mtx);

  assert(this->m_mapNewFunc.find(name) == this->m_mapNewFunc.end());

  PairFuncNewExchange p(name, func);
  auto ret = this->m_mapNewFunc.insert(p);
  assert(ret.second);

  LOG(INFO) << "ExchangeMgr::regNewExchange " << name;
}

void ExchangeMgr::deleteExchange(Exchange* pExchange) {
  std::lock_guard<std::mutex> lock(this->m_mtx);

  assert(pExchange != NULL);

  auto it = this->m_exchanges.find(pExchange);
  if (it != this->m_exchanges.end()) {
    this->m_exchanges.erase(it);
  }

  delete pExchange;
}

Exchange* ExchangeMgr::newExchange(const char* name) {
  std::lock_guard<std::mutex> lock(this->m_mtx);

  assert(this->m_pCfg != NULL);

  auto e = this->newExchange(name, *this->m_pCfg);

  this->m_exchanges.insert(e);

  return e;
}

void ExchangeMgr::releaseAllExchanges() {
  std::lock_guard<std::mutex> lock(this->m_mtx);

  for (auto it = this->m_exchanges.begin(); it != this->m_exchanges.end();
       ++it) {
    delete *it;
  }

  this->m_exchanges.clear();
}

CR2END