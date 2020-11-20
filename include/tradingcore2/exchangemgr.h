#ifndef __TRADINGCORE2_EXCHANGEMGR_H__
#define __TRADINGCORE2_EXCHANGEMGR_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/exchange.h>

#include <functional>
#include <map>
#include <mutex>
#include <set>
#include <string>

CR2BEGIN

// FuncNewExchange - new Exchange()
typedef std::function<Exchange*(const Config& cfg)> FuncNewExchange;

class ExchangeMgr {
 public:
  typedef std::pair<std::string, FuncNewExchange> PairFuncNewExchange;
  typedef std::map<std::string, FuncNewExchange> MapFuncNewExchange;
  typedef std::set<Exchange*> SetExchange;

 public:
  static ExchangeMgr* getSingleton();

 protected:
  ExchangeMgr() : m_pCfg(NULL) {}
  ~ExchangeMgr() { this->releaseAllExchanges(); }

 public:
  void init(const Config& cfg);

  void regNewExchange(const char* name, FuncNewExchange func);

 public:
  Exchange* newExchange(const char* name);

  void deleteExchange(Exchange* pExchange);

  void releaseAllExchanges();

 protected:
  Exchange* newExchange(const char* name, const Config& cfg);

 protected:
  std::mutex m_mtx;
  MapFuncNewExchange m_mapNewFunc;
  const Config* m_pCfg;
  SetExchange m_exchanges;
};

CR2END

#endif  // __TRADINGCORE2_EXCHANGEMGR_H__