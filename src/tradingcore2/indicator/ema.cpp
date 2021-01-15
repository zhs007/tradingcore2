
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

bool IndicatorEMA::build2(Exchange& exchange, const char* assetsName,
                          const char* assetsName2, IndicatorBuild2Type b2t,
                          int64_t ot, int start, int length) {
  assert(assetsName != NULL);
  assert(assetsName2 != NULL);
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

  auto pBuf = new IndicatorDataValue[length];

  m_iStart = start;

  CandleData cd;
  auto isok = exchange.getData(assetsName, start, cd);
  assert(isok);
  pBuf[0] = cd.close;

  CandleData cd1;
  isok = exchange.getDataWithTimestamp(assetsName2,
                                       cd.ts + this->m_params.b2OffTime, cd1);
  assert(isok);
  this->pushData(cd.ts, cd1.close);

  for (int i = 1; i < length; ++i) {
    isok = exchange.getData(assetsName, start + i, cd);
    assert(isok);
    pBuf[i] = (2 * cd.close + (this->m_avgtimes - 1) * pBuf[i - 1]) /
              (this->m_avgtimes + 1);

    CandleData cd1;
    isok = exchange.getDataWithTimestamp(assetsName2,
                                         cd.ts + this->m_params.b2OffTime, cd1);
    assert(isok);

    this->pushData(cd.ts,
                   (2 * cd1.close + (this->m_avgtimes - 1) * pBuf[i - 1]) /
                       (this->m_avgtimes + 1));
  }

  delete[] pBuf;

  return true;
}

const IndicatorData_singleValue* IndicatorEMA::getMinSingleValue(
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

const IndicatorData_singleValue* IndicatorEMA::getMaxSingleValue(
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

// newIndicator - new IndicatorEMA
Indicator* IndicatorEMA::newIndicator(const char* fullname,
                                      const char* assetsName) {
  return new IndicatorEMA(fullname, assetsName);
}

// isMine - isMine
bool IndicatorEMA::isMine(const char* name) {
  std::vector<std::string> arr;
  splitStr(arr, name, ".");

  if (arr.size() == 2 && arr[0] == "ema") {
    try {
      auto v = std::stoi(arr[1]);
      return true;
    } catch (...) {
      return false;
    }
  }

  return false;
}

CR2END