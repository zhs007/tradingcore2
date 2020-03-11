#include <tradingcore2/candle.h>

CR2BEGIN

void Candle::push(Price open, Price close, Price high, Price low, Volume volume,
                  Volume openInterest, time_t curtime) {
  CandleData c;

  c.open = open;
  c.close = close;
  c.high = high;
  c.low = low;
  c.volume = volume;
  c.openInterest = openInterest;
  c.curtime = curtime;

  m_lst.push_back(c);
}

CR2END