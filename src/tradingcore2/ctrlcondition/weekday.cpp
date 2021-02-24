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

bool CCWeekDay::canCtrl(const Exchange& exchange, const Wallet& wallet,
                        const IndicatorMap& mapIndicators,
                        const tradingpb::CtrlCondition& cc, bool issim,
                        CtrlType ct, TimeStamp ts, int index, CandleData& cd,
                        Assets& assets, void* pData) {
  tm ctm;
  timestamp2timeUTC(ts, ctm);

  // LOG(INFO) << "CCWeekDay2 " << ts << " " << ctm.tm_wday << " " <<
  // cc.vals(0);

  if (ctm.tm_wday == cc.vals(0)) {
    return true;
  }

  return false;
}

CR2END