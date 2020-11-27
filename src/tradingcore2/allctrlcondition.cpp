#include <tradingcore2/allctrlcondition.h>
#include <tradingcore2/ctrlcondition/buyandhold.h>
#include <tradingcore2/ctrlconditionmgr.h>

CR2BEGIN

void regAllCtrlConditionHelper() { CCBuyAndHold::regCtrlConditionHelper(); }

CR2END