#include <assert.h>
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

  Money price;
  Volume volume;
  TimeStamp ts;
  auto isok = exchange.getData(assetsName, start, ts, price, volume);
  assert(isok);

  IndicatorDataValue pv = price;

  this->pushData(ts, price);

  for (int i = 1; i < length; ++i) {
    auto isok = exchange.getData(assetsName, start + i, ts, price, volume);
    assert(isok);

    pv = (pv * (this->m_avgtimes - 1) + price) / this->m_avgtimes;
    this->pushData(ts, pv);
  }

  return true;
}

CR2END