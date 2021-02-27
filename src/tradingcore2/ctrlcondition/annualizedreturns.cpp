#include <math.h>
#include <tradingcore2/ctrlcondition/annualizedreturns.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/utils.h>
#include <tradingcore2/wallet.h>

#include <functional>

CR2BEGIN

void CCAnnualizedReturns::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("annualizedreturns",
                                                     new CCAnnualizedReturns());
}

bool CCAnnualizedReturns::isValid(const tradingpb::CtrlCondition& cc,
                                  CtrlType ct) {
  return cc.vals_size() == 1 && cc.operators_size() == 1;
}

bool CCAnnualizedReturns::canCtrl(const Exchange& exchange,
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
  auto tr = curvalue / cost;
  auto ar = tr;

  if (ts - pMyData->startTs >= 365 * 60 * 60 * 24) {
    auto yoff = (ts - pMyData->startTs) / (365 * 60 * 60 * 24);
    ar = (tr - 1) / (yoff + 1) + 1;
  }

  if (cc.operators(0) == ">=") {
    if (ar >= cc.vals(0)) {
      pMyData->isProc = true;

      return true;
    }
  }

  if (cc.operators(0) == ">") {
    if (ar > cc.vals(0)) {
      pMyData->isProc = true;

      return true;
    }
  }

  return false;
}

CR2END