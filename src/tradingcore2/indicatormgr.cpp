#include <math.h>
#include <tradingcore2/indicatormgr.h>
#include <tradingcore2/utils.h>

#include <functional>

CR2BEGIN

IndicatorMgr* IndicatorMgr::getSingleton() {
  // for c++ 11
  static IndicatorMgr s_mgr;

  return &s_mgr;
}

void IndicatorMgr::regIndicator(const char* name,
                                FuncNewIndicator funcNewIndicator,
                                FuncIsMime funcIsMine) {
  assert(this->m_map.find(name) == this->m_map.end());

  Pair p;
  p.first = name;
  p.second.newIndicator = funcNewIndicator;
  p.second.isMine = funcIsMine;

  auto ret = this->m_map.insert(p);
  assert(ret.second);
}

Indicator* IndicatorMgr::newIndicator(const char* name) {
  std::vector<std::string> arr;
  splitStr(arr, name, ".");

  auto it = this->m_map.find(arr[0]);
  if (it != this->m_map.end()) {
    if (it->second.isMine(name)) {
      return it->second.newIndicator(name);
    }
  }

  for (auto it = this->m_map.begin(); it != this->m_map.end(); ++it) {
    if (it->second.isMine(name)) {
      return it->second.newIndicator(name);
    }
  }

  return NULL;
}

CR2END