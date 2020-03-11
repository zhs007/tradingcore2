#include <tradingcore2/candle.h>

CR2BEGIN

void Candle::push(Money open, Money close, Money high, Money low, Volume volume,
                  Volume openInterest, TimeStamp ts) {
  CandleData c;

  c.open = open;
  c.close = close;
  c.high = high;
  c.low = low;
  c.volume = volume;
  c.openInterest = openInterest;
  c.ts = ts;

  m_lst.push_back(c);
}

CR2END