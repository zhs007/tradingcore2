
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
  CandleData cd;
  auto isok = exchange.getData(assetsName, start, cd);
  assert(isok);

  totalPrice = cd.close;
  this->pushData(cd.ts, totalPrice);

  for (int i = 1; i < length; ++i) {
    auto isok = exchange.getData(assetsName, start + i, cd);
    assert(isok);

    totalPrice += cd.close;
    this->pushData(cd.ts, totalPrice / (i + 1));
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

  if (this->m_avgtimes == 1) {
    return this->_build_avg1(exchange, assetsName, start, length);
  }

  m_iStart = start;

  if (this->m_avgtimes >= length) {
    Money tp;
    this->_buildFirst(exchange, assetsName, start, length, tp);

    return true;
  }

  Money tp;
  this->_buildFirst(exchange, assetsName, start, this->m_avgtimes, tp);

  CandleData cd;
  for (int i = this->m_avgtimes; i < length; ++i) {
    auto isok = exchange.getData(assetsName, start + i - this->m_avgtimes, cd);
    assert(isok);

    tp -= cd.close;

    isok = exchange.getData(assetsName, start + i, cd);
    assert(isok);

    tp += cd.close;

    this->pushData(cd.ts, tp / this->m_avgtimes);
  }

  return true;
}

bool IndicatorSMA::_build_avg1(Exchange& exchange, const char* assetsName,
                               int start, int length) {
  assert(assetsName != NULL);
  assert(start >= 0);
  assert(length > 0);
  assert(this->m_avgtimes == 1);

  m_iStart = start;

  CandleData cd;
  for (int i = 0; i < length; ++i) {
    auto isok = exchange.getData(assetsName, start + i, cd);
    assert(isok);

    this->pushData(cd.ts, cd.close);
  }

  return true;
}

// NewIndicatorSMA - new IndicatorSMA
Indicator* NewIndicatorSMA(int avgtimes) { return new IndicatorSMA(avgtimes); }

CR2END