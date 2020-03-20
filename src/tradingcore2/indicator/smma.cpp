
#include <stdlib.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/indicator/smma.h>
#include <tradingcore2/utils.h>

#include <ctime>
#include <iomanip>

CR2BEGIN

void IndicatorSMMA::pushData(TimeStamp ts, IndicatorDataValue val) {
  Node n;

  n.ts = ts;
  n.val.value = val;

  m_lst.push_back(n);
}

bool IndicatorSMMA::build(Exchange& exchange, const char* assetsName, int start,
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

  IndicatorDataValue pv = cd.close;

  this->pushData(cd.ts, cd.close);

  for (int i = 1; i < length; ++i) {
    auto isok = exchange.getData(assetsName, start + i, cd);
    assert(isok);

    pv = (pv * (this->m_avgtimes - 1) + cd.close) / this->m_avgtimes;
    this->pushData(cd.ts, pv);
  }

  return true;
}

// NewIndicatorSMMA - new IndicatorSMMA
Indicator* NewIndicatorSMMA(int avgtimes) {
  return new IndicatorSMMA(avgtimes);
}

CR2END