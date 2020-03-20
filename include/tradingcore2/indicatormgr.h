#ifndef __TRADINGCORE2_INDICATORMGR_H__
#define __TRADINGCORE2_INDICATORMGR_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/indicator.h>

#include <functional>
#include <map>
#include <string>

CR2BEGIN

// FuncNewIndicatorWithAvgTimes - new Indicator(int avgtime)
typedef std::function<Indicator*(int avgtimes)> FuncNewIndicatorWithAvgTimes;

class IndicatorMgr {
 public:
  typedef std::pair<std::string, FuncNewIndicatorWithAvgTimes> PairWithAvgTimes;
  typedef std::map<std::string, FuncNewIndicatorWithAvgTimes> MapWithAvgTimes;

 public:
  static IndicatorMgr* getSingleton();

 protected:
  IndicatorMgr() {}
  ~IndicatorMgr() {}

 public:
  void regIndicatorWithAvgTimes(const char* name,
                                FuncNewIndicatorWithAvgTimes func);

 public:
  Indicator* newIndicator(const char* name, int avgtimes);

 protected:
  MapWithAvgTimes m_mapWithAvgTimes;
};

CR2END

#endif  // __TRADINGCORE2_INDICATORMGR_H__