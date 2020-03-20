#include <math.h>
#include <tradingcore2/indicatormgr.h>

#include <functional>

CR2BEGIN

IndicatorMgr* IndicatorMgr::getSingleton() {
  // for c++ 11
  static IndicatorMgr s_mgr;

  return &s_mgr;
}

void IndicatorMgr::regIndicatorWithAvgTimes(const char* name,
                                            FuncNewIndicatorWithAvgTimes func) {
  assert(this->m_mapWithAvgTimes.find(name) == this->m_mapWithAvgTimes.end());

  PairWithAvgTimes p(name, func);
  auto ret = this->m_mapWithAvgTimes.insert(p);
  assert(ret.second);
}

Indicator* IndicatorMgr::newIndicator(const char* name, int avgtimes) {
  auto it = this->m_mapWithAvgTimes.find(name);
  if (it != this->m_mapWithAvgTimes.end()) {
    return it->second(avgtimes);
  }

  return NULL;
}

CR2END