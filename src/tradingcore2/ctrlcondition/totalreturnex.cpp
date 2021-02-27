#include <math.h>
#include <tradingcore2/ctrlcondition/totalreturnex.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/utils.h>
#include <tradingcore2/wallet.h>

#include <functional>

CR2BEGIN

void CCTotalReturnEx::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("totalreturnex",
                                                     new CCTotalReturnEx());
}

bool CCTotalReturnEx::isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) {
  return cc.vals_size() == 2 && cc.operators_size() == 1;
}

bool CCTotalReturnEx::canCtrl(const Exchange& exchange, const Wallet& wallet,
                              const IndicatorMap& mapIndicators,
                              const tradingpb::CtrlCondition& cc, bool issim,
                              CtrlType ct, TimeStamp ts, int index,
                              CandleData& cd, Assets& assets, void* pData) {
  assert(pData != NULL);
  auto pMyData = static_cast<_Data*>(pData);
  if (pMyData->isProc) {
    return false;
  }

  Money cost = wallet.getCost();
  if (cost <= 0) {
    return false;
  }

  auto curvalue = cd.close * assets.volume;
  auto tr = curvalue / cost;

  if (tr > pMyData->maxPerValue) {
    pMyData->maxPerValue = tr;
  }

  if (pMyData->isWaitDown) {
    if (pMyData->maxPerValue - tr >= cc.vals(1)) {
      pMyData->isProc = true;

      return true;
    }
  } else {
    if (cc.operators(0) == ">=") {
      if (tr >= cc.vals(0)) {
        pMyData->isWaitDown = true;
      }
    }

    if (cc.operators(0) == ">") {
      if (tr > cc.vals(0)) {
        pMyData->isWaitDown = true;
      }
    }
  }

  return false;
}

CR2END