
#include <stdlib.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/indicator/ema.h>
#include <tradingcore2/utils.h>

#include <ctime>
#include <iomanip>

CR2BEGIN

void IndicatorEMA::pushData(TimeStamp ts, IndicatorDataValue val) {
  Node n;

  n.ts = ts;
  n.val.value = val;

  m_lst.push_back(n);
}

bool IndicatorEMA::build(Exchange& exchange, const char* assetsName, int start,
                         int length) {
  assert(assetsName != NULL);
  assert(start >= 0);
  assert(length > 0);

  this->release();

  int totalLength = exchange.getDataLength(assetsName);
  if (start >= totalLength) {
    return false;
  }

  if (start + length >= totalLength) {
    length = totalLength - start;
  }

  if (length <= 0) {
    return false;
  }

  m_iStart = start;

  CandleData cd;
  auto isok = exchange.getData(assetsName, start, cd);
  assert(isok);

  this->pushData(cd.ts, cd.close);

  for (int i = 1; i < length; ++i) {
    auto isok = exchange.getData(assetsName, start + i, cd);
    assert(isok);

    this->pushData(cd.ts, (2 * cd.close + (this->m_avgtimes - 1) *
                                              this->m_lst[i - 1].val.value) /
                              (this->m_avgtimes + 1));
  }

  return true;
}

// NewIndicatorEMA - new IndicatorEMA
Indicator* NewIndicatorEMA(int avgtimes) { return new IndicatorEMA(avgtimes); }

CR2END