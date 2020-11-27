#include <math.h>
#include <tradingcore2/ctrlcondition/buyandhold.h>
#include <tradingcore2/ctrlconditionmgr.h>

#include <functional>

CR2BEGIN

void CCBuyAndHold::regCtrlConditionHelper() {
  CtrlConditionMgr::getSingleton()->regCtrlCondition("buyandhold",
                                                     new CCBuyAndHold());
}

bool CCBuyAndHold::isValid(const tradingpb::CtrlCondition& cc) { return false; }

void CCBuyAndHold::procCtrl(const tradingpb::CtrlCondition& cc,
                            FuncOnCtrl onctrl) {}

CR2END