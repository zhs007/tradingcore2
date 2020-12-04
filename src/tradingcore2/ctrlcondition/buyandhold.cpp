#include <math.h>
#include <tradingcore2/ctrlcondition/buyandhold.h>
#include <tradingcore2/ctrlconditionmgr.h>

#include <functional>

CR2BEGIN

void CCBuyAndHold::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("buyandhold",
                                                     new CCBuyAndHold());
}

bool CCBuyAndHold::isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) {
  if (ct == CT_BUY) {
    return true;
  }

  return false;
}

bool CCBuyAndHold::canCtrl(const IndicatorMap& mapIndicators,
                           const tradingpb::CtrlCondition& cc, bool issim,
                           CtrlType ct, TimeStamp ts, int index, CandleData& cd,
                           void* pData) {
  if (ct == CT_BUY && index == 0) {
    return true;
  }

  return false;
}

CR2END