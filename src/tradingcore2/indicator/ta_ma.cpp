
#include <stdlib.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/indicator/ta_ma.h>
#include <tradingcore2/utils.h>

#include <ctime>
#include <iomanip>

CR2BEGIN

void IndicatorTA_MA::pushData(TimeStamp ts, IndicatorDataValue val) {
  Node n;

  n.ts = ts;
  n.val.value = val;

  m_lst.push_back(n);
}

// void IndicatorTAMA::_buildFirst(Exchange& exchange, const char* assetsName,
//                                 int start, int length, Money& totalPrice) {
//   CandleData cd;
//   auto isok = exchange.getData(assetsName, start, cd);
//   assert(isok);

//   totalPrice = cd.close;
//   this->pushData(cd.ts, totalPrice);

//   for (int i = 1; i < length; ++i) {
//     auto isok = exchange.getData(assetsName, start + i, cd);
//     assert(isok);

//     totalPrice += cd.close;
//     this->pushData(cd.ts, totalPrice / (i + 1));
//   }
// }

bool IndicatorTA_MA::build(Exchange& exchange, const char* assetsName,
                           int start, int length) {
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

  TA_Real* pPrice = new TA_Real[length];
  TA_Real* pOut = new TA_Real[length];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  CandleData cd;
  for (int i = 0; i < length; ++i) {
    auto isok = exchange.getData(assetsName, start + i, cd);
    assert(isok);

    pPrice[i] = cd.close;
  }

  auto retCode = TA_MA(0, length - 1, pPrice, this->m_avgtimes, m_maType,
                       &outBeg, &outNbElement, pOut);
  if (retCode != TA_SUCCESS) {
    LOG(ERROR) << "IndicatorTAMA:TA_MA " << retCode;

    delete[] pPrice;
    delete[] pOut;

    return false;
  }

  LOG(INFO) << "IndicatorTAMA:TA_MA length " << length << " outBeg " << outBeg
            << " outNbElement " << outNbElement;
  assert(outBeg == length - outNbElement);

  // CandleData cd;
  for (int i = 0; i < length; ++i) {
    auto isok = exchange.getData(assetsName, start + i - this->m_avgtimes, cd);
    assert(isok);

    if (i < outBeg) {
      this->pushData(cd.ts, cd.close);
    } else {
      this->pushData(cd.ts, pOut[i - outBeg]);
    }
  }

  delete[] pPrice;
  delete[] pOut;

  return true;
}

// bool IndicatorTAMA::_build_avg1(Exchange& exchange, const char* assetsName,
//                                 int start, int length) {
//   assert(assetsName != NULL);
//   assert(start >= 0);
//   assert(length > 0);
//   assert(this->m_avgtimes == 1);

//   m_iStart = start;

//   CandleData cd;
//   for (int i = 0; i < length; ++i) {
//     auto isok = exchange.getData(assetsName, start + i, cd);
//     assert(isok);

//     this->pushData(cd.ts, cd.close);
//   }

//   return true;
// }

const IndicatorData_singleValue* IndicatorTA_MA::getMinSingleValue(
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

const IndicatorData_singleValue* IndicatorTA_MA::getMaxSingleValue(
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

// newIndicator - new IndicatorTA_MA
Indicator* IndicatorTA_MA::newIndicator(int avgtimes) {
  return new IndicatorTA_MA(avgtimes);
}

CR2END