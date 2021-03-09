#include <math.h>
#include <tradingcore2/ctrlcondition/indicatordv.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/indicatormap.h>

#include <functional>

CR2BEGIN

// 因为是数值相关模式，和 indicatorsv 比较，唯一的区别就是2个数值指标比较，且
// 都是 第一种 对 第二种的。

void CCIndicatorDV::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("indicatordv",
                                                     new CCIndicatorDV());
}

void CCIndicatorDV::getIndicators(std::set<std::string>& indicators,
                                  const tradingpb::CtrlCondition& cc) {
  indicators.insert(cc.strvals(0));
  indicators.insert(cc.strvals(1));
}

bool CCIndicatorDV::isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) {
  return cc.strvals_size() == 2 && cc.operators_size() == 1;
}

bool CCIndicatorDV::canCtrl(const Exchange& exchange, const Wallet& wallet,
                            const IndicatorMap& mapIndicators,
                            const tradingpb::CtrlCondition& cc, bool issim,
                            CtrlType ct, TimeStamp ts, int index,
                            CandleData& cd, Assets& assets, void* pData) {
  auto pIndicator0 = mapIndicators.getIndicator(cc.strvals(0).c_str());
  auto pIndicator1 = mapIndicators.getIndicator(cc.strvals(1).c_str());
  if (pIndicator0 != NULL && pIndicator1 != NULL) {
    // assert(pData != NULL);
    auto cv0 = pIndicator0->getSingleValue(index);
    assert(cv0 != NULL);

    auto cv1 = pIndicator1->getSingleValue(index);
    assert(cv1 != NULL);

    auto pMyData = static_cast<_Data*>(pData);

    bool canctrl = false;

    if (cc.operators(0) == "==") {
      if (cv0->value == cv1->value) {
        canctrl = true;
      }
    } else if (cc.operators(0) == "<") {
      if (cv0->value < cv1->value) {
        canctrl = true;
      }
    } else if (cc.operators(0) == "<=") {
      if (cv0->value <= cv1->value) {
        canctrl = true;
      }
    } else if (cc.operators(0) == ">") {
      if (cv0->value > cv1->value) {
        canctrl = true;
      }
    } else if (cc.operators(0) == ">=") {
      if (cv0->value >= cv1->value) {
        canctrl = true;
      }
    }

    return canctrl;
  } else {
    LOG(INFO) << "no indicator " << cc.strvals(0) << " || " << cc.strvals(1);
  }

  return false;
}

CR2END