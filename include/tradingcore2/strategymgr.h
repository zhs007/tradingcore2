#ifndef __TRADINGCORE2_STRATEGYMGR_H__
#define __TRADINGCORE2_STRATEGYMGR_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/pnl.h>
#include <tradingcore2/protos/trading2.pb.h>

#include <map>
#include <string>
#include <vector>

CR2BEGIN

class Strategy;

// StrategyMgr -
class StrategyMgr {
 public:
  typedef std::function<Strategy*(Wallet& wallet, Exchange& exchange)>
      FuncNewStrategy;
  typedef std::pair<std::string, FuncNewStrategy> PairStrategy;
  typedef std::map<std::string, FuncNewStrategy> MapStrategy;
  typedef std::set<Strategy*> SetStrategy;

 public:
  static StrategyMgr* getSingleton();

 public:
  void regStrategy(const char* name, StrategyMgr::FuncNewStrategy funcNew);

  Strategy* newStrategy(const char* name, Wallet& wallet, Exchange& exchange);

  void deleteStrategy(Strategy* strategy);

 protected:
  StrategyMgr() {}
  ~StrategyMgr() {}

 protected:
  void release();

 protected:
  MapStrategy m_mapStrategy;
  SetStrategy m_setStrategy;
};

CR2END

#endif  // __TRADINGCORE2_STRATEGYMGR_H__