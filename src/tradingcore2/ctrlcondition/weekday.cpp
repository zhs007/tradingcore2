#include <math.h>
#include <tradingcore2/ctrlcondition/weekday.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/utils.h>

#include <functional>

CR2BEGIN

void CCWeekDay::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("weekday",
                                                     new CCWeekDay());
}

bool CCWeekDay::isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) {
  return cc.vals_size() == 1;
}

void CCWeekDay::procCtrl(const IndicatorMap& mapIndicators,
                         const tradingpb::CtrlCondition& cc, bool issim,
                         CtrlType ct, TimeStamp ts, int index, CandleData& cd,
                         void* pData, FuncOnCtrl onctrl) {
  tm ctm;
  timestamp2timeUTC(ts, ctm);

  if (ctm.tm_wday == cc.vals(0) && onctrl != NULL) {
    onctrl(issim, ct, ts);
  }
}

CR2END