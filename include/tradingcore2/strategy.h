#ifndef __TRADINGCORE2_STRATEGY_H__
#define __TRADINGCORE2_STRATEGY_H__

#include <tradingcore2/basedef.h>
#include <tradingcore2/pnl.h>

#include <map>
#include <string>
#include <vector>

CR2BEGIN

class Strategy {
 public:
  Strategy(Wallet& wallet, Exchange& exchange)
      : m_wallet(wallet), m_exchange(exchange) {}
  virtual ~Strategy() {}

 public:
  virtual void onTimeStamp(TimeStamp ts, int index) = 0;

 protected:
  Wallet& m_wallet;
  Exchange& m_exchange;
  PNL m_pnl;
};

CR2END

#endif  // __TRADINGCORE2_STRATEGY_H__