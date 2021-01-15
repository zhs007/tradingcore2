#include <math.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/indicatormap.h>
#include <tradingcore2/indicatormgr.h>
#include <tradingcore2/utils.h>

#include <functional>

CR2BEGIN

void IndicatorMap::addIndicator(const char* fullname, const char* asset) {
  auto it = this->m_map.find(fullname);
  if (it != this->m_map.end()) {
    return;
  }

  // std::vector<std::string> arr;
  // splitStr(arr, fullname, ".");

  // if (arr.size() == 2) {
  //   auto v = atoi(arr[1].c_str());

  auto pIndicator = IndicatorMgr::getSingleton()->newIndicator(fullname, asset);

  PairIndicator p(fullname, pIndicator);
  auto ret = this->m_map.insert(p);
  assert(ret.second);
  // }
}

void IndicatorMap::build(Exchange& exchange, const char* asset) {
  for (auto it = this->m_map.begin(); it != this->m_map.end(); ++it) {
    it->second->build(exchange, asset, 0, exchange.getDataLength(asset));
  }
}

void IndicatorMap::release() {
  for (auto it = this->m_map.begin(); it != this->m_map.end(); ++it) {
    delete it->second;
  }

  this->m_map.clear();
}

const Indicator* IndicatorMap::getIndicator(const char* fullname) const {
  auto it = this->m_map.find(fullname);
  if (it != this->m_map.end()) {
    return it->second;
  }

  return NULL;
}

CR2END