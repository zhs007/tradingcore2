#include <math.h>
#include <tradingcore2/ctrlcondition/timestamp.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/utils.h>

#include <functional>

CR2BEGIN

void CCTimestamp::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("timestamp",
                                                     new CCTimestamp());
}

bool CCTimestamp::isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) {
  return cc.int64vals_size() == 1 && cc.operators_size() == 1;
}

bool CCTimestamp::canCtrl(const Exchange& exchange, const Wallet& wallet,
                          const IndicatorMap& mapIndicators,
                          const tradingpb::CtrlCondition& cc, bool issim,
                          CtrlType ct, TimeStamp ts, int index, CandleData& cd,
                          Assets& assets, void* pData) {
  assert(pData != NULL);
  auto pMyData = static_cast<_Data*>(pData);
  if (pMyData->isProc) {
    return false;
  }

  if (cc.operators(0) == "==") {
    if (ts == cc.int64vals(0)) {
      pMyData->isProc = true;

      return true;
    }
  }

  if (cc.operators(0) == ">=") {
    if (ts >= cc.int64vals(0)) {
      pMyData->isProc = true;

      return true;
    }
  }

  if (cc.operators(0) == ">") {
    if (ts > cc.int64vals(0)) {
      pMyData->isProc = true;

      return true;
    }
  }

  return false;
}

CR2END