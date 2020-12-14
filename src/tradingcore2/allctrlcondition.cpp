#include <tradingcore2/allctrlcondition.h>
#include <tradingcore2/ctrlcondition/buyandhold.h>
#include <tradingcore2/ctrlcondition/indicatorsp.h>
#include <tradingcore2/ctrlcondition/indicatorsv.h>
#include <tradingcore2/ctrlcondition/monthday.h>
#include <tradingcore2/ctrlcondition/monthdayex.h>
#include <tradingcore2/ctrlcondition/weekday.h>
#include <tradingcore2/ctrlcondition/weekday2.h>
#include <tradingcore2/ctrlcondition/weekdayex.h>
#include <tradingcore2/ctrlconditionmgr.h>

CR2BEGIN

void regAllCtrlConditionHelper() {
  CCBuyAndHold::regCtrlConditionHelper();
  CCWeekDay::regCtrlConditionHelper();
  CCWeekDay2::regCtrlConditionHelper();
  CCMonthDay::regCtrlConditionHelper();
  CCWeekDayEx::regCtrlConditionHelper();
  CCMonthDayEx::regCtrlConditionHelper();
  CCIndicatorSV::regCtrlConditionHelper();
  CCIndicatorSP::regCtrlConditionHelper();
}

CR2END