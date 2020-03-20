
#include <stdlib.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/indicator/sma.h>
#include <tradingcore2/utils.h>

#include <ctime>
#include <iomanip>

CR2BEGIN

void IndicatorSMA::pushData(TimeStamp ts, IndicatorDataValue val) {
  Node n;

  n.ts = ts;
  n.val.value = val;

  m_lst.push_back(n);
}

void IndicatorSMA::_buildFirst(Exchange& exchange, const char* assetsName,
                               int start, int length, Money& totalPrice) {
  Money price;
  Volume volume;
  TimeStamp ts;
  auto isok = exchange.getData(assetsName, start, ts, price, volume);
  assert(isok);

  totalPrice = price;
  this->pushData(ts, totalPrice);

  for (int i = 1; i < length; ++i) {
    auto isok = exchange.getData(assetsName, start + i, ts, price, volume);
    assert(isok);

    totalPrice += price;
    this->pushData(ts, totalPrice / (i + 1));
  }
}

bool IndicatorSMA::build(Exchange& exchange, const char* assetsName, int start,
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

  if (this->m_avgtimes >= length) {
    Money tp;
    this->_buildFirst(exchange, assetsName, start, length, tp);

    return true;
  }

  Money tp;
  this->_buildFirst(exchange, assetsName, start, this->m_avgtimes, tp);

  for (int i = this->m_avgtimes; i < length; ++i) {
    Money price;
    Volume volume;
    TimeStamp ts;

    auto isok = exchange.getData(assetsName, start + i - this->m_avgtimes, ts,
                                 price, volume);
    assert(isok);

    tp -= price;

    isok = exchange.getData(assetsName, start + i, ts, price, volume);
    assert(isok);

    tp += price;

    this->pushData(ts, tp / this->m_avgtimes);
  }

  return true;
}

CR2END