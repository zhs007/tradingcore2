
#include <stdlib.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/indicator/wma.h>
#include <tradingcore2/utils.h>

#include <ctime>
#include <iomanip>

CR2BEGIN

void IndicatorWMA::pushData(TimeStamp ts, IndicatorDataValue val) {
  Node n;

  n.ts = ts;
  n.val.value = val;

  m_lst.push_back(n);
}

Money IndicatorWMA::_getPrice(Exchange& exchange, const char* assetsName,
                              int start, int index) {
  assert(start >= 0);

  CandleData cd;

  if (index <= 0) {
    auto isok = exchange.getData(assetsName, start, cd);
    assert(isok);

    return cd.close;
  }

  auto isok = exchange.getData(assetsName, start + index, cd);
  assert(isok);

  return cd.close;
}

bool IndicatorWMA::build(Exchange& exchange, const char* assetsName, int start,
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

    Money tp = cd.close * this->m_avgtimes;

    for (int j = this->m_avgtimes - 1; j > 0; --j) {
      Money cp = this->_getPrice(exchange, assetsName, start,
                                 i - this->m_avgtimes + j);
      tp += j * cp;
    }

    this->pushData(cd.ts, tp / (m_avgtimes * (m_avgtimes + 1) / 2));
  }

  return true;
}

const IndicatorData_singleValue* IndicatorWMA::getMinSingleValue(
    int& index) const {
  const IndicatorData_singleValue* pMin = NULL;

  for (auto it = this->m_lst.begin(); it != this->m_lst.end(); ++it) {
    if (it->val.value != 100 && (pMin == NULL || it->val.value < pMin->value)) {
      pMin = &(it->val);
      index = std::distance(it, this->m_lst.begin());
    }
  }

  return pMin;
}

const IndicatorData_singleValue* IndicatorWMA::getMaxSingleValue(
    int& index) const {
  const IndicatorData_singleValue* pMax = NULL;

  for (auto it = this->m_lst.begin(); it != this->m_lst.end(); ++it) {
    if (it->val.value != 100 && (pMax == NULL || it->val.value > pMax->value)) {
      pMax = &(it->val);
      index = std::distance(it, this->m_lst.begin());
    }
  }

  return pMax;
}

// NewIndicatorWMA - new IndicatorWMA
Indicator* NewIndicatorWMA(int avgtimes) { return new IndicatorWMA(avgtimes); }

CR2END