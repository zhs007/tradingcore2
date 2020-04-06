#include <math.h>
#include <tradingcore2/exchangemgr.h>

#include <functional>

CR2BEGIN

ExchangeMgr* ExchangeMgr::getSingleton() {
  // for c++ 11
  static ExchangeMgr s_mgr;

  return &s_mgr;
}

Exchange* ExchangeMgr::getExchange(const char* name) {
  auto it = this->m_mapExchange.find(name);
  if (it != this->m_mapExchange.end()) {
    return it->second;
  }

  return NULL;
}

Exchange* ExchangeMgr::newExchange(const char* name, Config& cfg) {
  auto it = this->m_mapNewFunc.find(name);
  assert(it != this->m_mapNewFunc.end());

  auto pExchange = it->second(cfg);
  this->addExchange(pExchange);
}

void ExchangeMgr::addExchange(Exchange* pExchange) {
  assert(this->m_mapExchange.find(pExchange->getTypeName()) ==
         this->m_mapExchange.end());

  PairExchange p(pExchange->getTypeName(), pExchange);
  auto ret = this->m_mapExchange.insert(p);
  assert(ret.second);
}

void ExchangeMgr::init(Config& cfg) {
  assert(this->m_mapExchange.size() == 0);

  for (auto it = cfg.exchanges.begin(); it != cfg.exchanges.end(); ++it) {
    this->newExchange(it->c_str(), cfg);
  }
}

void ExchangeMgr::regNewExchange(const char* name, FuncNewExchange func) {
  assert(this->m_mapNewFunc.find(name) == this->m_mapNewFunc.end());

  PairFuncNewExchange p(name, func);
  auto ret = this->m_mapNewFunc.insert(p);
  assert(ret.second);
}

CR2END