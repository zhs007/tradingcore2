#include <math.h>
#include <tradingcore2/ctrlcondition/annualizedreturnsex.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/utils.h>
#include <tradingcore2/wallet.h>

#include <functional>

CR2BEGIN

void CCAnnualizedReturnsEx::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition(
      "annualizedreturnsex", new CCAnnualizedReturnsEx());
}

bool CCAnnualizedReturnsEx::isValid(const tradingpb::CtrlCondition& cc,
                                    CtrlType ct) {
  return cc.vals_size() == 1 && cc.operators_size() == 1;
}

bool CCAnnualizedReturnsEx::canCtrl(const Exchange& exchange,
                                    const Wallet& wallet,
                                    const IndicatorMap& mapIndicators,
                                    const tradingpb::CtrlCondition& cc,
                                    bool issim, CtrlType ct, TimeStamp ts,
                                    int index, CandleData& cd, Assets& assets,
                                    void* pData) {
  assert(pData != NULL);
  auto pMyData = static_cast<_Data*>(pData);
  if (pMyData->isProc) {
    return false;
  }

  if (pMyData->startTs <= 0) {
    pMyData->startTs = ts;
  }

  Money cost = wallet.getCost();
  if (cost <= 0) {
    return false;
  }

  auto curvalue = cd.close * assets.volume;
  auto ttr = curvalue / cost;
  auto ar = ttr;

  if (ts - pMyData->startTs >= 365 * 60 * 60 * 24) {
    auto yoff = (ts - pMyData->startTs) / (365 * 60 * 60 * 24);
    ar = (ttr - 1) / (yoff + 1) + 1;
  }

  if (ar > pMyData->maxPerValue) {
    pMyData->maxPerValue = ar;
  }

  if (pMyData->isWaitDown) {
    if (pMyData->maxPerValue - ar >= cc.vals(1)) {
      pMyData->isProc = true;

      return true;
    }
  } else {
    if (cc.operators(0) == ">=") {
      if (ar >= cc.vals(0)) {
        pMyData->isWaitDown = true;
      }
    }

    if (cc.operators(0) == ">") {
      if (ar > cc.vals(0)) {
        pMyData->isWaitDown = true;
      }
    }
  }

  return false;
}

CR2END