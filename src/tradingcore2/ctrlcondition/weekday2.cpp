#include <math.h>
#include <tradingcore2/ctrlcondition/weekday2.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/utils.h>

#include <functional>

CR2BEGIN

void CCWeekDay2::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("weekday2",
                                                     new CCWeekDay2());
}

bool CCWeekDay2::isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) {
  // 需要2个值，第一个是周几买，第二个是间隔几天后依然是有效交易日
  return cc.vals_size() == 2;
}

bool CCWeekDay2::canCtrl(const Exchange& exchange, const Wallet& wallet,
                         const IndicatorMap& mapIndicators,
                         const tradingpb::CtrlCondition& cc, bool issim,
                         CtrlType ct, TimeStamp ts, int index, CandleData& cd,
                         void* pData) {
  tm ctm;
  timestamp2timeUTC(ts, ctm);

  if (ctm.tm_wday == cc.vals(0) &&
      exchange.isValidTs(ts + cc.vals(1) * 24 * 60 * 60)) {
    return true;
  }

  return false;
}

CR2END