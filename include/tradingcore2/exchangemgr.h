#ifndef __TRADINGCORE2_EXCHANGEMGR_H__
#define __TRADINGCORE2_EXCHANGEMGR_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/exchange.h>

#include <functional>
#include <map>
#include <string>

CR2BEGIN

// FuncNewExchange - new Exchange()
typedef std::function<Exchange*(const Config& cfg)> FuncNewExchange;

class ExchangeMgr {
 public:
  typedef std::pair<std::string, FuncNewExchange> PairFuncNewExchange;
  typedef std::map<std::string, FuncNewExchange> MapFuncNewExchange;
  typedef std::pair<std::string, Exchange*> PairExchange;
  typedef std::map<std::string, Exchange*> MapExchange;

 public:
  static ExchangeMgr* getSingleton();

 protected:
  ExchangeMgr() {}
  ~ExchangeMgr() {}

 public:
  void init(const Config& cfg);

  void regNewExchange(const char* name, FuncNewExchange func);

 public:
  Exchange* getExchange(const char* name);

 protected:
  Exchange* newExchange(const char* name, const Config& cfg);

  void addExchange(Exchange* pExchange);

 protected:
  MapFuncNewExchange m_mapNewFunc;
  MapExchange m_mapExchange;
};

CR2END

#endif  // __TRADINGCORE2_EXCHANGEMGR_H__