#include <math.h>
#include <tradingcore2/ctrlcondition/indicatorsv.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/indicatormap.h>

#include <functional>

CR2BEGIN

// 因为是单值模式，需要判断当前值和配置值之间的关系

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

bool CCIndicatorSV::canCtrl(const Exchange& exchange, const Wallet& wallet,
                            const IndicatorMap& mapIndicators,
                            const tradingpb::CtrlCondition& cc, bool issim,
                            CtrlType ct, TimeStamp ts, int index,
                            CandleData& cd, Assets& assets, void* pData) {
  auto pIndicator = mapIndicators.getIndicator(cc.strvals(0).c_str());
  if (pIndicator != NULL) {
    auto cv = pIndicator->getSingleValue(index);
    assert(cv != NULL);

    bool canctrl = false;

    if (cc.operators(0) == "=" || cc.operators(0) == "==") {
      if (cv->value == cc.vals(0)) {
        canctrl = true;
      }
    } else if (cc.operators(0) == ">") {
      if (cv->value > cc.vals(0)) {
        canctrl = true;
      }
    } else if (cc.operators(0) == ">=") {
      if (cv->value >= cc.vals(0)) {
        canctrl = true;
      }
    } else if (cc.operators(0) == "<") {
      if (cv->value < cc.vals(0)) {
        canctrl = true;
      }
    } else if (cc.operators(0) == "<=") {
      if (cv->value <= cc.vals(0)) {
        canctrl = true;
      }
    }

    return canctrl;
  } else {
    LOG(INFO) << "no indicator " << cc.strvals(0);
  }

  return false;
}

CR2END