#include <math.h>
#include <tradingcore2/strategy.h>
#include <tradingcore2/strategymgr.h>

#include <functional>

CR2BEGIN

StrategyMgr* StrategyMgr::getSingleton() {
  // for c++ 11
  static StrategyMgr s_mgr;

  return &s_mgr;
}

void StrategyMgr::regStrategy(const char* name,
                              StrategyMgr::FuncNewStrategy funcNew) {
  PairStrategy p(name, funcNew);

  auto ret = this->m_mapStrategy.insert(p);
  assert(ret.second);
}

Strategy* StrategyMgr::newStrategy(const char* name, Wallet& wallet,
                                   Exchange& exchange) {
  auto it = this->m_mapStrategy.find(name);
  if (it != this->m_mapStrategy.end()) {
    return it->second(wallet, exchange);
  }

  return NULL;
}

void StrategyMgr::release() {
  for (auto it = this->m_setStrategy.begin(); it != this->m_setStrategy.end();
       ++it) {
    delete *it;
  }

  this->m_setStrategy.clear();
  this->m_mapStrategy.clear();
}

void StrategyMgr::deleteStrategy(Strategy* strategy) {
  auto it = this->m_setStrategy.find(strategy);
  if (it != this->m_setStrategy.end()) {
    delete *it;

    this->m_setStrategy.erase(it);
  }
}

CR2END