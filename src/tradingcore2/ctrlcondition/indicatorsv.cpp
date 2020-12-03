#include <math.h>
#include <tradingcore2/ctrlcondition/indicatorsv.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/indicatormap.h>

#include <functional>

CR2BEGIN

void CCIndicatorSV::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("indicatorsv",
                                                     new CCIndicatorSV());
}

void CCIndicatorSV::getIndicators(std::set<std::string>& indicators,
                                const tradingpb::CtrlCondition& cc) {
  indicators.insert(cc.strvals(0));
}

bool CCIndicatorSV::isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) {
  return cc.vals_size() == 1 && cc.strvals_size() == 1 &&
         cc.operators_size() == 1;
}

void CCIndicatorSV::procCtrl(const IndicatorMap& mapIndicators,
                           const tradingpb::CtrlCondition& cc, bool issim,
                           CtrlType ct, TimeStamp ts, int index, void* pData,
                           FuncOnCtrl onctrl) {
  auto pIndicator = mapIndicators.getIndicator(cc.strvals(0).c_str());
  if (pIndicator != NULL) {
    auto cv = pIndicator->getSingleValue(index);
    assert(cv != NULL);

    if (cc.operators(0) == "=" || cc.operators(0) == "==") {
      if (cv->value == cc.vals(0) && onctrl != NULL) {
        onctrl(issim, ct, ts);
      }
    } else if (cc.operators(0) == ">") {
      if (cv->value > cc.vals(0) && onctrl != NULL) {
        onctrl(issim, ct, ts);
      }
    } else if (cc.operators(0) == ">=") {
      if (cv->value >= cc.vals(0) && onctrl != NULL) {
        onctrl(issim, ct, ts);
      }
    } else if (cc.operators(0) == "<") {
      if (cv->value < cc.vals(0) && onctrl != NULL) {
        onctrl(issim, ct, ts);
      }
    } else if (cc.operators(0) == "<=") {
      if (cv->value <= cc.vals(0) && onctrl != NULL) {
        onctrl(issim, ct, ts);
      }
    }
  } else {
    LOG(INFO) << "no indicator " << cc.strvals(0);
  }
}

CR2END