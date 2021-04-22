
#include <stdlib.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/indicator/ta_roc.h>
#include <tradingcore2/utils.h>

#include <ctime>
#include <iomanip>

CR2BEGIN

void IndicatorTA_ROC::pushData(TimeStamp ts, IndicatorDataValue val) {
  Node n;

  n.ts = ts;
  n.val.value = val;

  m_lst.push_back(n);
}

bool IndicatorTA_ROC::build(Exchange& exchange, const char* assetsName,
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

  // TA_RetCode TA_ROC( int    startIdx,
  //                    int    endIdx,
  //                    const double inReal[],
  //                    int           optInTimePeriod, /* From 1 to 100000 */
  //                    int          *outBegIdx,
  //                    int          *outNBElement,
  //                    double        outReal[] );

  auto retCode = TA_ROC(0, length - 1, pPrice, this->m_avgtimes, &outBeg,
                        &outNbElement, pOut);
  if (retCode != TA_SUCCESS) {
    LOG(ERROR) << "IndicatorTAMA:TA_ROC " << retCode;

    delete[] pPrice;
    delete[] pOut;

    return false;
  }

  //   LOG(INFO) << "IndicatorTA_ROC:TA_ROC length " << length << " outBeg " <<
  //   outBeg
  //             << " outNbElement " << outNbElement;
  assert(outBeg == length - outNbElement);

  // CandleData cd;
  for (int i = 0; i < length; ++i) {
    auto isok = exchange.getData(assetsName, start + i, cd);
    assert(isok);

    if (i < outBeg) {
      this->pushData(cd.ts, 0);
    } else {
      this->pushData(cd.ts, pOut[i - outBeg]);
    }
  }

  delete[] pPrice;
  delete[] pOut;

  return true;
}

bool IndicatorTA_ROC::build2(Exchange& exchange, const char* assetsName,
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

  TA_Real* pPrice = new TA_Real[length];
  TA_Real* pOut = new TA_Real[length];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  CandleData cd;
  for (int ii = 0; ii < length; ++ii) {
    auto isok = exchange.getData(assetsName, start + ii, cd);
    assert(isok);

    if (ii >= this->m_avgtimes - 1) {
      CandleData cd1;
      isok = exchange.getDataWithTimestamp(
          assetsName2, cd.ts + this->m_params.b2OffTime, cd1);
      if (!isok) {
        if (ii != length - 1) {
          LOG(WARNING) << "IndicatorTA_ROC:getDataWithTimestamp " << ii << " "
                       << cd.ts + this->m_params.b2OffTime;
        }
        // assert(ii == length - 1);

        cd1.close = cd.close;
      }

      pPrice[ii] = cd1.close;

      // TA_RetCode TA_ROC( int    startIdx,
      //                    int    endIdx,
      //                    const double inReal[],
      //                    int           optInTimePeriod, /* From 1 to 100000
      //                    */ int          *outBegIdx, int *outNBElement,
      //                    double        outReal[] );

      auto retCode = TA_ROC(ii - this->m_avgtimes + 1, ii, pPrice,
                            this->m_avgtimes, &outBeg, &outNbElement, pOut);
      if (retCode != TA_SUCCESS) {
        LOG(ERROR) << "IndicatorTA_ROC:TA_ROC " << retCode;

        delete[] pPrice;
        delete[] pOut;

        return false;
      }

      assert(outNbElement >= 1 && outNbElement <= this->m_avgtimes);
      assert(outBeg == this->m_avgtimes - 1 ||
             outBeg == ii - this->m_avgtimes + 1);
      // assert(outBeg == ii + 1 - (ii - this->m_avgtimes + 1) - outNbElement);
      // assert(outNbElement == 1);
      this->pushData(cd.ts, pOut[outNbElement - 1]);
    } else {
      this->pushData(cd.ts, 0);
    }

    pPrice[ii] = cd.close;
  }

  delete[] pPrice;
  delete[] pOut;

  return true;
}

const IndicatorData_singleValue* IndicatorTA_ROC::getMinSingleValue(
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

const IndicatorData_singleValue* IndicatorTA_ROC::getMaxSingleValue(
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

// newIndicator - new IndicatorTA_ROC
Indicator* IndicatorTA_ROC::newIndicator(const char* fullname,
                                         const char* assetsName) {
  std::vector<std::string> arr;
  splitStr(arr, fullname, ".");

  if (arr.size() == 2) {
    return new IndicatorTA_ROC(fullname, assetsName);
  }

  return NULL;
}

// isMine - isMine
bool IndicatorTA_ROC::isMine(const char* name) {
  std::vector<std::string> arr;
  splitStr(arr, name, ".");

  if (arr.size() == 2) {
    if (arr[0] == "ta-roc") {
      try {
        auto v = std::stoi(arr[1]);
        return v >= 1;
      } catch (...) {
        return false;
      }
    }
  }

  return false;
}

CR2END