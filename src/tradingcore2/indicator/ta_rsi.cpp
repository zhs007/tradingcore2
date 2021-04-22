
#include <stdlib.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/indicator/ta_rsi.h>
#include <tradingcore2/utils.h>

#include <ctime>
#include <iomanip>

CR2BEGIN

void IndicatorTA_RSI::pushData(TimeStamp ts, IndicatorDataValue val) {
  Node n;

  n.ts = ts;
  n.val.value = val;

  m_lst.push_back(n);
}

bool IndicatorTA_RSI::build(Exchange& exchange, const char* assetsName,
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

  // TA_RetCode TA_RSI( int    startIdx,
  //                int    endIdx,
  //                const double inReal[],
  //                int           optInTimePeriod, /* From 2 to 100000 */
  //                int          *outBegIdx,
  //                int          *outNBElement,
  //                double        outReal[] );

  auto retCode = TA_RSI(0, length - 1, pPrice, this->m_avgtimes, &outBeg,
                        &outNbElement, pOut);
  if (retCode != TA_SUCCESS) {
    LOG(ERROR) << "IndicatorTAMA:TA_RSI " << retCode;

    delete[] pPrice;
    delete[] pOut;

    return false;
  }

//   LOG(INFO) << "IndicatorTA_RSI:TA_RSI length " << length << " outBeg " << outBeg
//             << " outNbElement " << outNbElement;
  assert(outBeg == length - outNbElement);

  // CandleData cd;
  for (int i = 0; i < length; ++i) {
    auto isok = exchange.getData(assetsName, start + i, cd);
    assert(isok);

    if (i < outBeg) {
      this->pushData(cd.ts, 100);
    } else {
      this->pushData(cd.ts, pOut[i - outBeg]);
    }
  }

  delete[] pPrice;
  delete[] pOut;

  return true;
}

bool IndicatorTA_RSI::build2(Exchange& exchange, const char* assetsName,
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
          LOG(WARNING) << "IndicatorTA_RSI:getDataWithTimestamp " << ii << " "
                       << cd.ts + this->m_params.b2OffTime;
        }
        // assert(ii == length - 1);

        cd1.close = cd.close;
      }

      pPrice[ii] = cd1.close;

      // TA_RetCode TA_RSI( int    startIdx,
      //                int    endIdx,
      //                const double inReal[],
      //                int           optInTimePeriod, /* From 2 to 100000 */
      //                int          *outBegIdx,
      //                int          *outNBElement,
      //                double        outReal[] );

      auto retCode = TA_RSI(ii - this->m_avgtimes + 1, ii, pPrice,
                            this->m_avgtimes, &outBeg, &outNbElement, pOut);
      if (retCode != TA_SUCCESS) {
        LOG(ERROR) << "IndicatorTA_RSI:TA_MA " << retCode;

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
      this->pushData(cd.ts, 100);
    }

    pPrice[ii] = cd.close;
  }

  // for (int i = 0; i < length; ++i) {
  //   auto isok = exchange.getData(assetsName, start + i, cd);
  //   assert(isok);

  //   pPrice[i] = cd.close;
  // }

  // auto retCode = TA_MA(0, length - 1, pPrice, this->m_avgtimes, m_maType,
  //                      &outBeg, &outNbElement, pOut);
  // if (retCode != TA_SUCCESS) {
  //   LOG(ERROR) << "IndicatorTAMA:TA_MA " << retCode;

  //   delete[] pPrice;
  //   delete[] pOut;

  //   return false;
  // }

  // LOG(INFO) << "IndicatorTAMA:TA_MA length " << length << " outBeg " <<
  // outBeg
  //           << " outNbElement " << outNbElement;
  // assert(outBeg == length - outNbElement);

  // // CandleData cd;
  // for (int i = 0; i < length; ++i) {
  //   auto isok = exchange.getData(assetsName, start + i, cd);
  //   assert(isok);

  //   if (i < outBeg) {
  //     this->pushData(cd.ts, cd.close);
  //   } else {
  //     this->pushData(cd.ts, pOut[i - outBeg]);
  //   }
  // }

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

const IndicatorData_singleValue* IndicatorTA_RSI::getMinSingleValue(
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

const IndicatorData_singleValue* IndicatorTA_RSI::getMaxSingleValue(
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

// newIndicator - new IndicatorTA_RSI
Indicator* IndicatorTA_RSI::newIndicator(const char* fullname,
                                         const char* assetsName) {
  std::vector<std::string> arr;
  splitStr(arr, fullname, ".");

  if (arr.size() == 2) {
    return new IndicatorTA_RSI(fullname, assetsName);
  }

  return NULL;
}

// isMine - isMine
bool IndicatorTA_RSI::isMine(const char* name) {
  std::vector<std::string> arr;
  splitStr(arr, name, ".");

  if (arr.size() == 2) {
    if (arr[0] == "ta-rsi") {
      try {
        auto v = std::stoi(arr[1]);
        return true;
      } catch (...) {
        return false;
      }
    }
  }

  return false;
}

CR2END