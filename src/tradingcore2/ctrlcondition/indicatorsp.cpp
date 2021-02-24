#include <math.h>
#include <tradingcore2/ctrlcondition/indicatorsp.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/indicatormap.h>

#include <functional>

CR2BEGIN

// 因为是价格相关模式，这里只考虑close，且只有4种操作点
// 价格向上突破（upcross）、价格向下突破(downcross)
// 价格高于indicator (up)、价格低于indicator (down)

void CCIndicatorSP::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("indicatorsp",
                                                     new CCIndicatorSP());
}

void CCIndicatorSP::getIndicators(std::set<std::string>& indicators,
                                  const tradingpb::CtrlCondition& cc) {
  indicators.insert(cc.strvals(0));
}

bool CCIndicatorSP::isValid(const tradingpb::CtrlCondition& cc, CtrlType ct) {
  return cc.strvals_size() == 1 && cc.operators_size() == 1;
}

bool CCIndicatorSP::canCtrl(const Exchange& exchange, const Wallet& wallet,
                            const IndicatorMap& mapIndicators,
                            const tradingpb::CtrlCondition& cc, bool issim,
                            CtrlType ct, TimeStamp ts, int index,
                            CandleData& cd, Assets& assets, void* pData) {
  auto pIndicator = mapIndicators.getIndicator(cc.strvals(0).c_str());
  if (pIndicator != NULL) {
    auto cv = pIndicator->getSingleValue(index);
    assert(cv != NULL);

    assert(pData != NULL);
    auto pMyData = static_cast<_Data*>(pData);

    bool canctrl = false;

    if (cc.operators(0) == "up") {
      if (cd.close > cv->value) {
        canctrl = true;
      }
    } else if (cc.operators(0) == "down") {
      if (cd.close < cv->value) {
        canctrl = true;
      }
    } else if (cc.operators(0) == "upcross") {
      if (cd.close > cv->value && pMyData->lastState <= 0) {
        canctrl = true;
      }
    } else if (cc.operators(0) == "downcross") {
      if (cd.close < cv->value && pMyData->lastState >= 0) {
        canctrl = true;
      }
    }

    if (cd.close > cv->value) {
      pMyData->lastState = 1;
    } else if (cd.close < cv->value) {
      pMyData->lastState = -1;
    } else if (cd.close == cv->value) {
      pMyData->lastState = 0;
    }

    return canctrl;
  } else {
    LOG(INFO) << "no indicator " << cc.strvals(0);
  }

  return false;
}

CR2END