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

void CCMonthDayEx::procCtrl(const tradingpb::CtrlCondition& cc, bool issim,
                            CtrlType ct, TimeStamp ts, int index, void* pData,
                            FuncOnCtrl onctrl) {
  assert(pData != NULL);
  auto pMyData = static_cast<_Data*>(pData);

  auto md = calcMonthOffWithMonthDay(pMyData->lastTs, ts, cc.vals(0));

  if (md > 0) {
    if (onctrl != NULL) {
      onctrl(issim, ct, ts);
    }

    pMyData->lastTs = ts;
  }
}

CR2END