#include <math.h>
#include <tradingcore2/ctrlcondition/waittostart.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/utils.h>

#include <functional>

CR2BEGIN

void CCWaitToStart::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("waittostart",
                                                     new CCWaitToStart());
}

bool CCWaitToStart::isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) {
  return cc.vals_size() == 1;
}

bool CCWaitToStart::canCtrl(const Exchange& exchange, const Wallet& wallet,
                            const IndicatorMap& mapIndicators,
                            const tradingpb::CtrlCondition& cc, bool issim,
                            CtrlType ct, TimeStamp ts, int index,
                            CandleData& cd, Assets& assets, void* pData) {
  if (index >= cc.vals(0)) {
    return true;
  }

  return false;
}

CR2END