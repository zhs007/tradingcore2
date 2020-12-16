#include <math.h>
#include <tradingcore2/ctrlcondition/ctrlconditionid.h>
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/indicatormap.h>
#include <tradingcore2/wallet.h>

#include <functional>

CR2BEGIN

// 主要用于判断最后一次操作的ctrlConditionID的，一般用于多条件买入以后的卖出操作
// 可以用来只卖出特定条件的

void CCCtrlConditionID::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("ctrlconditionid",
                                                     new CCCtrlConditionID());
}

bool CCCtrlConditionID::isValid(const tradingpb::CtrlCondition& cc,
                                CtrlType ct) {
  return cc.strvals_size() == 1 && cc.vals_size() == 1;
}

bool CCCtrlConditionID::canCtrl(const Exchange& exchange, const Wallet& wallet,
                                const IndicatorMap& mapIndicators,
                                const tradingpb::CtrlCondition& cc, bool issim,
                                CtrlType ct, TimeStamp ts, int index,
                                CandleData& cd, void* pData) {
  if (cc.strvals(0) == "buy") {
    auto ln = wallet.getLastNode(TT_BUY);
    // if (ln != NULL) {
    //   LOG(INFO) << "CCCtrlConditionID " << cc.vals(0) << " "
    //             << ln->ctrlConditionID;
    // }

    if (ln != NULL && ln->ctrlConditionID == cc.vals(0)) {
      return true;
    }

    return false;
  } else {
    auto ln = wallet.getLastNode(TT_SELL);
    if (ln != NULL && ln->ctrlConditionID == cc.vals(0)) {
      return true;
    }

    return false;
  }

  return false;
}

CR2END