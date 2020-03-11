#ifndef __TRADINGCORE2_CANDLE_H__
#define __TRADINGCORE2_CANDLE_H__

#include <vector>
#include <tradingcore2/basedef.h>

CR2BEGIN

struct CandleData {
  time_t curtime;
  Price open, close, high, low;
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
  void push(Price open, Price close, Price high, Price low, Volume volume,
            Volume openInterest, time_t curtime);

 protected:
  List m_lst;
};

CR2END

#endif  // __TRADINGCORE2_CANDLE_H__