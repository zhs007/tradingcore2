#ifndef __TRADINGCORE2_CANDLE_H__
#define __TRADINGCORE2_CANDLE_H__

#include <tradingcore2/basedef.h>

#include <vector>

CR2BEGIN

struct CandleData {
  TimeStamp ts;
  Money open, close, high, low;
  Volume volume;
  Volume openInterest;

  CandleData()
      : ts(0), open(0), close(0), high(0), low(0), volume(0), openInterest(0) {}

  CandleData(TimeStamp cts, Money price)
      : ts(cts),
        open(price),
        close(price),
        high(price),
        low(price),
        volume(0),
        openInterest(0) {}
};

class Candle {
 public:
  typedef std::vector<CandleData> List;

 public:
  Candle() {}
  ~Candle() {}

 public:
  void push(Money open, Money close, Money high, Money low, Volume volume,
            Volume openInterest, time_t ts);

 protected:
  List m_lst;
};

CR2END

#endif  // __TRADINGCORE2_CANDLE_H__