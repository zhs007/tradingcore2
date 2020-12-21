
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
  CandleData cd;
  auto isok = exchange.getData(assetsName, start, cd);
  assert(isok);

  this->pushData(cd.ts, 0.0);

  for (int i = 1; i < length; ++i) {
    auto isok = exchange.getData(assetsName, start + 1, cd);
    assert(isok);

    this->pushData(cd.ts, 0.0);
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
    CandleData cd;
    Money lastPrice;

    auto isok = exchange.getData(assetsName, start + i - this->m_avgtimes, cd);
    assert(isok);

    lastPrice = cd.close;

    isok = exchange.getData(assetsName, start + i, cd);
    assert(isok);

    if (lastPrice == ZEROMONEY) {
      this->pushData(cd.ts, 0.0);
    } else {
      this->pushData(cd.ts, (cd.close - lastPrice) / lastPrice);
    }
  }

  return true;
}

const IndicatorData_singleValue* IndicatorROC::getMinSingleValue(
    int& index) const {
  const IndicatorData_singleValue* pMin = NULL;

  for (auto it = this->m_lst.begin(); it != this->m_lst.end(); ++it) {
    if (pMin == NULL || it->val.value < pMin->value) {
      pMin = &(it->val);
      index = std::distance(it, this->m_lst.begin());
    }
  }

  return pMin;
}

const IndicatorData_singleValue* IndicatorROC::getMaxSingleValue(
    int& index) const {
  const IndicatorData_singleValue* pMax = NULL;

  for (auto it = this->m_lst.begin(); it != this->m_lst.end(); ++it) {
    if (pMax == NULL || it->val.value > pMax->value) {
      pMax = &(it->val);
      index = std::distance(it, this->m_lst.begin());
    }
  }

  return pMax;
}

// newIndicator - new IndicatorROC
Indicator* IndicatorROC::newIndicator(int avgtimes) {
  return new IndicatorROC(avgtimes);
}

CR2END