#ifndef __TRADINGCORE2_INDICATORMGR_H__
#define __TRADINGCORE2_INDICATORMGR_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/indicator.h>

#include <functional>
#include <map>
#include <string>

CR2BEGIN

// FuncNewIndicator - newIndicator(const char*)
typedef std::function<Indicator*(const char*, const char*)> FuncNewIndicator;
// FuncIsMime - bool isMine(const char*)
typedef std::function<bool(const char*)> FuncIsMime;

class IndicatorMgr {
 public:
  struct IndicatorFuns {
    FuncNewIndicator newIndicator;
    FuncIsMime isMine;
  };

  typedef std::pair<std::string, IndicatorFuns> Pair;
  typedef std::map<std::string, IndicatorFuns> Map;

 public:
  static IndicatorMgr* getSingleton();

 protected:
  IndicatorMgr() {}
  ~IndicatorMgr() {}

 public:
  void regIndicator(const char* name, FuncNewIndicator funcNewIndicator,
                    FuncIsMime funcIsMine);

 public:
  Indicator* newIndicator(const char* fullname, const char* assetsName);

 protected:
  Map m_map;
};

CR2END

#endif  // __TRADINGCORE2_INDICATORMGR_H__