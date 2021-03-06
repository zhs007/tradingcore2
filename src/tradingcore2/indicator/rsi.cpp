
#include <stdlib.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/indicator/rsi.h>
#include <tradingcore2/utils.h>

#include <ctime>
#include <iomanip>

CR2BEGIN

void IndicatorRSI::pushData(TimeStamp ts, IndicatorDataValue u,
                            IndicatorDataValue d, IndicatorDataValue smmau,
                            IndicatorDataValue smmad, IndicatorDataValue val) {
  Node n;

  n.ts = ts;
  n.u = u;
  n.d = d;
  n.smmau = smmau;
  n.smmad = smmad;
  n.val.value = val;

  m_lst.push_back(n);
}

bool IndicatorRSI::build(Exchange& exchange, const char* assetsName, int start,
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

  Money preprice = cd.close;
  IndicatorDataValue presmmau = 0;
  IndicatorDataValue presmmad = 0;

  this->pushData(cd.ts, 0.0, 0.0, 0.0, 0.0, 100);

  for (int i = 1; i < length; ++i) {
    auto isok = exchange.getData(assetsName, start + i, cd);
    assert(isok);

    IndicatorDataValue u, d, smmau, smmad;

    if (cd.close > preprice) {
      u = cd.close - preprice;
      d = 0;
    } else {
      u = 0;
      d = preprice - cd.close;
    }

    smmau = (presmmau * (this->m_avgtimes - 1) + u) / this->m_avgtimes;
    smmad = (presmmad * (this->m_avgtimes - 1) + d) / this->m_avgtimes;

    IndicatorDataValue rsi = 100;
    if (smmad != 0) {
      rsi = 100 - 100 / (1 + smmau / smmad);
    }

    this->pushData(cd.ts, u, d, smmau, smmad, rsi);

    preprice = cd.close;
    presmmau = smmau;
    presmmad = smmad;
  }

  return true;
}

bool IndicatorRSI::build2(Exchange& exchange, const char* assetsName,
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

  m_iStart = start;

  CandleData cd;
  auto isok = exchange.getData(assetsName, start, cd);
  assert(isok);

  Money preprice = cd.close;
  IndicatorDataValue presmmau = 0;
  IndicatorDataValue presmmad = 0;

  this->pushData(cd.ts, 0.0, 0.0, 0.0, 0.0, 100);

  for (int i = 1; i < length; ++i) {
    auto isok = exchange.getData(assetsName, start + i, cd);
    assert(isok);

    IndicatorDataValue u, d, smmau, smmad;

    if (cd.close > preprice) {
      u = cd.close - preprice;
      d = 0;
    } else {
      u = 0;
      d = preprice - cd.close;
    }

    smmau = (presmmau * (this->m_avgtimes - 1) + u) / this->m_avgtimes;
    smmad = (presmmad * (this->m_avgtimes - 1) + d) / this->m_avgtimes;

    CandleData cd1;
    isok = exchange.getDataWithTimestamp(assetsName2,
                                         cd.ts + this->m_params.b2OffTime, cd1);
    assert(isok);

    IndicatorDataValue u1, d1, smmau1, smmad1;

    if (cd1.close > preprice) {
      u1 = cd1.close - preprice;
      d1 = 0;
    } else {
      u1 = 0;
      d1 = preprice - cd1.close;
    }

    smmau1 = (presmmau * (this->m_avgtimes - 1) + u1) / this->m_avgtimes;
    smmad1 = (presmmad * (this->m_avgtimes - 1) + d1) / this->m_avgtimes;

    IndicatorDataValue rsi = 100;
    if (smmad1 != 0) {
      rsi = 100 - 100 / (1 + smmau1 / smmad1);
    }

    this->pushData(cd.ts, u1, d1, smmau1, smmad1, rsi);

    preprice = cd.close;
    presmmau = smmau;
    presmmad = smmad;
  }

  return true;
}

const IndicatorData_singleValue* IndicatorRSI::getMinSingleValue(
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

const IndicatorData_singleValue* IndicatorRSI::getMaxSingleValue(
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

// newIndicator - new IndicatorRSI
Indicator* IndicatorRSI::newIndicator(const char* fullname,
                                      const char* assetsName) {
  return new IndicatorRSI(fullname, assetsName);
}

// isMine - isMine
bool IndicatorRSI::isMine(const char* name) {
  std::vector<std::string> arr;
  splitStr(arr, name, ".");

  if (arr.size() == 2 && arr[0] == "rsi") {
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