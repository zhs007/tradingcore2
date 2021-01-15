
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

void IndicatorROC::_buildFirst2(Exchange& exchange, const char* assetsName,
                                const char* assetsName2,
                                IndicatorBuild2Type b2t, int64_t ot, int start,
                                int length, IndicatorDataValue* pBuf) {
  CandleData cd;
  auto isok = exchange.getData(assetsName, start, cd);
  assert(isok);
  pBuf[0] = 0.0;

  this->pushData(cd.ts, 0.0);

  for (int i = 1; i < length; ++i) {
    isok = exchange.getData(assetsName, start + 1, cd);
    assert(isok);

    pBuf[i] = 0.0;

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

bool IndicatorROC::build2(Exchange& exchange, const char* assetsName,
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

  // auto pBuf = new IndicatorDataValue[length];

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

    CandleData cd1;
    isok = exchange.getDataWithTimestamp(assetsName2,
                                         cd.ts + this->m_params.b2OffTime, cd1);
    assert(isok);

    if (lastPrice == ZEROMONEY) {
      this->pushData(cd.ts, 0.0);
    } else {
      this->pushData(cd.ts, (cd1.close - lastPrice) / lastPrice);
    }
  }

  // delete[] pBuf;

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
Indicator* IndicatorROC::newIndicator(const char* fullname,
                                      const char* assetsName) {
  return new IndicatorROC(fullname, assetsName);
}

// isMine - isMine
bool IndicatorROC::isMine(const char* name) {
  std::vector<std::string> arr;
  splitStr(arr, name, ".");

  if (arr.size() == 2 && arr[0] == "roc") {
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