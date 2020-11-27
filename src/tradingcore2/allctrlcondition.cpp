#include <tradingcore2/allctrlcondition.h>
#include <tradingcore2/ctrlcondition/buyandhold.h>
#include <tradingcore2/ctrlcondition/monthday.h>
#include <tradingcore2/ctrlcondition/weekday.h>
#include <tradingcore2/ctrlconditionmgr.h>

CR2BEGIN

void regAllCtrlConditionHelper() {
  CCBuyAndHold::regCtrlConditionHelper();
  CCWeekDay::regCtrlConditionHelper();
  CCMonthDay::regCtrlConditionHelper();
}

CR2END