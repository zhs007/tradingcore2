
#include <stdlib.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/indicator/roc.h>
#include <tradingcore2/utils.h>

#include <ctime>
#include <iomanip>

CR2BEGIN

void IndicatorROC::pushData(TimeStamp ts, IndicatorDataValue val) {
  Node n;

  n.ts = ts;
  n.val.value = val;

  m_lst.push_back(n);
}

void IndicatorROC::_buildFirst(Exchange& exchange, const char* assetsName,
                               int start, int length) {
  Money price;
  Volume volume;
  TimeStamp ts;
  auto isok = exchange.getData(assetsName, start, ts, price, volume);
  assert(isok);

  this->pushData(ts, 0.0);

  for (int i = 1; i < length; ++i) {
    auto isok = exchange.getData(assetsName, start + 1, ts, price, volume);
    assert(isok);

    this->pushData(ts, 0.0);
  }
}

bool IndicatorROC::build(Exchange& exchange, const char* assetsName, int start,
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

  if (this->m_avgtimes > length) {
    this->_buildFirst(exchange, assetsName, start, length);

    return true;
  }

  this->_buildFirst(exchange, assetsName, start, this->m_avgtimes);

  for (int i = this->m_avgtimes; i < length; ++i) {
    Money price;
    Volume volume;
    TimeStamp ts;
    Money lastPrice;

    auto isok = exchange.getData(assetsName, start + i - this->m_avgtimes, ts,
                                 lastPrice, volume);
    assert(isok);

    isok = exchange.getData(assetsName, start + i, ts, price, volume);
    assert(isok);

    if (lastPrice == ZEROMONEY) {
      this->pushData(ts, 0.0);
    } else {
      this->pushData(ts, (price - lastPrice) / lastPrice);
    }
  }

  return true;
}

// NewIndicatorROC - new IndicatorROC
Indicator* NewIndicatorROC(int avgtimes) { return new IndicatorROC(avgtimes); }

CR2END