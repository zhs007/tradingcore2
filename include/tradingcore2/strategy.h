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
      : m_wallet(wallet),
        m_exchange(exchange),
        m_stoploss(-1),
        m_curStopLossPrice(-1),
        m_tradingNums(0),
        m_stoplossNums(0) {}
  virtual ~Strategy() {}

 public:
  virtual void onTimeStamp(TimeStamp ts, int index) = 0;

 public:
  void setStopLess(float stoploss) { m_stoploss = stoploss; }

 public:
  void simulateTrading();

  void print();

 protected:
  void onSimulateTradingTimeStamp(TimeStamp ts, int index);

  void setStopLossPrice(Money price) { m_curStopLossPrice = price; }

  Money onProcStopLoss(const char* assetsName, Money curPrice, Volume volume,
                       TimeStamp ts, int index);

  void onTrading() { m_tradingNums++; }

 protected:
  Wallet& m_wallet;
  Exchange& m_exchange;
  PNL m_pnl;

  float m_stoploss;
  Money m_curStopLossPrice;

  int m_tradingNums;
  int m_stoplossNums;
};

CR2END

#endif  // __TRADINGCORE2_STRATEGY_H__