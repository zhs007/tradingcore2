#include <math.h>
#include <tradingcore2/ctrlcondition/totalreturn.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/utils.h>
#include <tradingcore2/wallet.h>

#include <functional>

CR2BEGIN

void CCTotalReturn::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("totalreturn",
                                                     new CCTotalReturn());
}

bool CCTotalReturn::isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) {
  return cc.vals_size() == 1 && cc.operators_size() == 1;
}

bool CCTotalReturn::canCtrl(const Exchange& exchange, const Wallet& wallet,
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

  if (cc.operators(0) == ">=") {
    if (tr >= cc.vals(0)) {
      pMyData->isProc = true;

      return true;
    }
  }

  if (cc.operators(0) == ">") {
    if (tr > cc.vals(0)) {
      pMyData->isProc = true;

      return true;
    }
  }

  return false;
}

CR2END