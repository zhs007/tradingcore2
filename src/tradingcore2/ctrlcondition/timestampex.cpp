#include <math.h>
#include <tradingcore2/ctrlcondition/timestampex.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/utils.h>

#include <functional>

CR2BEGIN

void CCTimestampEx::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("timestampex",
                                                     new CCTimestampEx());
}

bool CCTimestampEx::isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) {
  return cc.int64vals_size() >= 1;
}

bool CCTimestampEx::canCtrl(const Exchange& exchange, const Wallet& wallet,
                            const IndicatorMap& mapIndicators,
                            const tradingpb::CtrlCondition& cc, bool issim,
                            CtrlType ct, TimeStamp ts, int index,
                            CandleData& cd, Assets& assets, void* pData) {
  assert(pData != NULL);
  auto pMyData = static_cast<_Data*>(pData);
  if (pMyData->curIndex >= cc.int64vals_size()) {
    return false;
  }

  if (ts == cc.int64vals(pMyData->curIndex)) {
    pMyData->curIndex++;

    return true;
  }

  return false;
}

CR2END