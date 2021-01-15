#ifndef __TRADINGCORE2_INDICATORMAP_H__
#define __TRADINGCORE2_INDICATORMAP_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/indicator.h>

#include <map>
#include <string>

CR2BEGIN

class IndicatorMap {
 public:
  typedef std::pair<std::string, Indicator*> PairIndicator;
  typedef std::map<std::string, Indicator*> MapIndicator;

 public:
  IndicatorMap() {}
  ~IndicatorMap() {}

 public:
  void addIndicator(const char* fullname, const char* asset);

  void build(Exchange& exchange, const char* asset);

  void release();

  const Indicator* getIndicator(const char* fullname) const;

 protected:
  MapIndicator m_map;
};

CR2END

#endif  // __TRADINGCORE2_INDICATORMAP_H__