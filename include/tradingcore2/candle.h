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