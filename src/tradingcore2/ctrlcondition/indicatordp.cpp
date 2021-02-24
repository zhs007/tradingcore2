#include <math.h>
#include <tradingcore2/ctrlcondition/indicatordp.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/indicatormap.h>

#include <functional>

CR2BEGIN

// 因为是价格相关模式，和 indicatorsp 比较，唯一的区别就是2个价格指标比较，且
// 都是 第一种 对 第二种的。只有4种操作点
// 价格向上突破（upcross）、价格向下突破(downcross)
// 价格高于indicator (up)、价格低于indicator (down)

void CCIndicatorDP::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("indicatordp",
                                                     new CCIndicatorDP());
}

void CCIndicatorDP::getIndicators(std::set<std::string>& indicators,
                                  const tradingpb::CtrlCondition& cc) {
  indicators.insert(cc.strvals(0));
  indicators.insert(cc.strvals(1));
}

bool CCIndicatorDP::isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) {
  return cc.strvals_size() == 2 && cc.operators_size() == 1;
}

bool CCIndicatorDP::canCtrl(const Exchange& exchange, const Wallet& wallet,
                            const IndicatorMap& mapIndicators,
                            const tradingpb::CtrlCondition& cc, bool issim,
                            CtrlType ct, TimeStamp ts, int index,
                            CandleData& cd, Assets& assets, void* pData) {
  auto pIndicator0 = mapIndicators.getIndicator(cc.strvals(0).c_str());
  auto pIndicator1 = mapIndicators.getIndicator(cc.strvals(1).c_str());
  if (pIndicator0 != NULL && pIndicator1 != NULL) {
    assert(pData != NULL);
    auto cv0 = pIndicator0->getSingleValue(index);
    assert(cv0 != NULL);

    auto cv1 = pIndicator1->getSingleValue(index);
    assert(cv1 != NULL);

    auto pMyData = static_cast<_Data*>(pData);

    bool canctrl = false;

    if (cc.operators(0) == "up") {
      if (cv0->value > cv1->value) {
        canctrl = true;
      }
    } else if (cc.operators(0) == "down") {
      if (cv0->value < cv1->value) {
        canctrl = true;
      }
    } else if (cc.operators(0) == "upcross") {
      if (cv0->value > cv1->value && pMyData->lastState <= 0) {
        canctrl = true;
      }
    } else if (cc.operators(0) == "downcross") {
      if (cv0->value < cv1->value && pMyData->lastState >= 0) {
        canctrl = true;
      }
    }

    if (cv0->value > cv1->value) {
      pMyData->lastState = 1;
    } else if (cv0->value < cv1->value) {
      pMyData->lastState = -1;
    } else if (cv0->value == cv1->value) {
      pMyData->lastState = 0;
    }

    return canctrl;
  } else {
    LOG(INFO) << "no indicator " << cc.strvals(0) << " || " << cc.strvals(1);
  }

  return false;
}

CR2END