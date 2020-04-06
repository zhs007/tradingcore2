#ifndef __TRADINGCORE2_EXCHANGEMGR_H__
#define __TRADINGCORE2_EXCHANGEMGR_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/exchange.h>

#include <functional>
#include <map>
#include <string>

CR2BEGIN

// FuncNewIndicatorWithAvgTimes - new Indicator(int avgtime)
typedef std::function<Indicator*(int avgtimes)> FuncNewIndicatorWithAvgTimes;

class ExchangeMgr {
 public:
  typedef std::pair<std::string, FuncNewIndicatorWithAvgTimes> PairWithAvgTimes;
  typedef std::map<std::string, FuncNewIndicatorWithAvgTimes> MapWithAvgTimes;

 public:
  static ExchangeMgr* getSingleton();

 protected:
  ExchangeMgr() {}
  ~ExchangeMgr() {}

 public:
  void regIndicatorWithAvgTimes(const char* name,
                                FuncNewIndicatorWithAvgTimes func);

 public:
  Indicator* newIndicator(const char* name, int avgtimes);

 protected:
  MapWithAvgTimes m_mapWithAvgTimes;
};

CR2END

#endif  // __TRADINGCORE2_EXCHANGEMGR_H__