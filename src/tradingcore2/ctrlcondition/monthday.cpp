#include <math.h>
#include <tradingcore2/ctrlcondition/monthday.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/utils.h>

#include <functional>

CR2BEGIN

void CCMonthDay::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("monthday",
                                                     new CCMonthDay());
}

bool CCMonthDay::isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) {
  return cc.vals_size() == 1;
}

void CCMonthDay::procCtrl(const tradingpb::CtrlCondition& cc, bool issim,
                          CtrlType ct, TimeStamp ts, int index,
                          FuncOnCtrl onctrl) {
  tm ctm;
  timestamp2timeUTC(ts, ctm);

  if (ctm.tm_mday == cc.vals(0) && onctrl != NULL) {
    onctrl(issim, ct, ts);
  }
}

CR2END