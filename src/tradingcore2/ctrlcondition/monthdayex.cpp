#include <math.h>
#include <tradingcore2/ctrlcondition/monthdayex.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/utils.h>

#include <functional>

CR2BEGIN

void CCMonthDayEx::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("monthdayex",
                                                     new CCMonthDayEx());
}

bool CCMonthDayEx::isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) {
  return cc.vals_size() == 1;
}

bool CCMonthDayEx::canCtrl(const Exchange& exchange,
                           const IndicatorMap& mapIndicators,
                           const tradingpb::CtrlCondition& cc, bool issim,
                           CtrlType ct, TimeStamp ts, int index, CandleData& cd,
                           void* pData) {
  assert(pData != NULL);
  auto pMyData = static_cast<_Data*>(pData);

  auto md = calcMonthOffWithMonthDay(pMyData->lastTs, ts, cc.vals(0));

  if (md > 0) {
    pMyData->lastTs = ts;

    return true;
  }

  return false;
}

CR2END