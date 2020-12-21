#include <math.h>
#include <tradingcore2/ctrlcondition/month.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/utils.h>

#include <functional>

CR2BEGIN

void CCMonth::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("month", new CCMonth());
}

bool CCMonth::isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) {
  return cc.vals_size() == 1;
}

bool CCMonth::canCtrl(const Exchange& exchange, const Wallet& wallet,
                      const IndicatorMap& mapIndicators,
                      const tradingpb::CtrlCondition& cc, bool issim,
                      CtrlType ct, TimeStamp ts, int index, CandleData& cd,
                      void* pData) {
  tm ctm;
  timestamp2timeUTC(ts, ctm);

  if (ctm.tm_mon == cc.vals(0)) {
    return true;
  }

  return false;
}

CR2END