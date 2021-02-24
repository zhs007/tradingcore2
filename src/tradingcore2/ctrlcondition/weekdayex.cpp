#include <math.h>
#include <tradingcore2/ctrlcondition/weekdayex.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/utils.h>

#include <functional>

CR2BEGIN

void CCWeekDayEx::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("weekdayex",
                                                     new CCWeekDayEx());
}

bool CCWeekDayEx::isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) {
  return cc.vals_size() == 1;
}

bool CCWeekDayEx::canCtrl(const Exchange& exchange, const Wallet& wallet,
                          const IndicatorMap& mapIndicators,
                          const tradingpb::CtrlCondition& cc, bool issim,
                          CtrlType ct, TimeStamp ts, int index, CandleData& cd,
                          Assets& assets, void* pData) {
  assert(pData != NULL);
  auto pMyData = static_cast<_Data*>(pData);

  auto wd = calcWeekOffWithWeekDay(pMyData->lastTs, ts, cc.vals(0));

  if (wd > 0) {
    pMyData->lastTs = ts;

    return true;
  }

  return false;
}

CR2END