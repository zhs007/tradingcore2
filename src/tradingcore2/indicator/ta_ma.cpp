
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

  // LOG(INFO) << "IndicatorTAMA:TA_MA length " << length << " outBeg " << outBeg
  //           << " outNbElement " << outNbElement;
  assert(outBeg == length - outNbElement);

  // CandleData cd;
  for (int i = 0; i < length; ++i) {
    auto isok = exchange.getData(assetsName, start + i, cd);
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

bool IndicatorTA_MA::build2(Exchange& exchange, const char* assetsName,
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
          LOG(WARNING) << "IndicatorTAMA:getDataWithTimestamp " << ii << " "
                       << cd.ts + this->m_params.b2OffTime;
        }
        // assert(ii == length - 1);

        cd1.close = cd.close;
      }

      pPrice[ii] = cd1.close;

      auto retCode =
          TA_MA(ii - this->m_avgtimes + 1, ii, pPrice, this->m_avgtimes,
                m_maType, &outBeg, &outNbElement, pOut);
      if (retCode != TA_SUCCESS) {
        LOG(ERROR) << "IndicatorTAMA:TA_MA " << retCode;

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
      this->pushData(cd.ts, cd.close);
    }

    pPrice[ii] = cd.close;
  }

  delete[] pPrice;
  delete[] pOut;

  return true;
}

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
Indicator* IndicatorTA_MA::newIndicator(const char* fullname,
                                        const char* assetsName) {
  std::vector<std::string> arr;
  splitStr(arr, fullname, ".");

  if (arr.size() == 2) {
    TA_MAType maType;
    if (arr[0] == "ta-sma") {
      maType = TA_MAType_SMA;
    } else if (arr[0] == "ta-ema") {
      maType = TA_MAType_EMA;
    } else if (arr[0] == "ta-wma") {
      maType = TA_MAType_WMA;
    } else if (arr[0] == "ta-dema") {
      maType = TA_MAType_DEMA;
    } else if (arr[0] == "ta-tema") {
      maType = TA_MAType_TEMA;
    } else if (arr[0] == "ta-trima") {
      maType = TA_MAType_TRIMA;
    } else if (arr[0] == "ta-kama") {
      maType = TA_MAType_KAMA;
    } else if (arr[0] == "ta-mama") {
      maType = TA_MAType_MAMA;
    } else if (arr[0] == "ta-t3") {
      maType = TA_MAType_T3;
    }

    return new IndicatorTA_MA(maType, fullname, assetsName);
  }

  return NULL;
}

// isMine - isMine
bool IndicatorTA_MA::isMine(const char* name) {
  std::vector<std::string> arr;
  splitStr(arr, name, ".");

  if (arr.size() == 2) {
    if (arr[0] == "ta-sma" || arr[0] == "ta-ema" || arr[0] == "ta-wma" ||
        arr[0] == "ta-dema" || arr[0] == "ta-tema" || arr[0] == "ta-trima" ||
        arr[0] == "ta-kama" || arr[0] == "ta-mama" || arr[0] == "ta-t3") {
      try {
        auto v = std::stoi(arr[1]);
        return v >= 2;
      } catch (...) {
        return false;
      }
    }
  }

  return false;
}

CR2END