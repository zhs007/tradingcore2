#include <tradingcore2/allctrlcondition.h>
#include <tradingcore2/ctrlcondition/buyandhold.h>
#include <tradingcore2/ctrlcondition/ctrlconditionid.h>
#include <tradingcore2/ctrlcondition/indicatordp.h>
#include <tradingcore2/ctrlcondition/indicatorsp.h>
#include <tradingcore2/ctrlcondition/indicatorsp2.h>
#include <tradingcore2/ctrlcondition/indicatorsv.h>
#include <tradingcore2/ctrlcondition/month.h>
#include <tradingcore2/ctrlcondition/monthday.h>
#include <tradingcore2/ctrlcondition/monthdayex.h>
#include <tradingcore2/ctrlcondition/waittostart.h>
#include <tradingcore2/ctrlcondition/weekday.h>
#include <tradingcore2/ctrlcondition/weekday2.h>
#include <tradingcore2/ctrlcondition/weekdayex.h>
#include <tradingcore2/ctrlconditionmgr.h>

CR2BEGIN

void regAllCtrlConditionHelper() {
  CCBuyAndHold::regCtrlConditionHelper();
  CCWeekDay::regCtrlConditionHelper();
  CCWeekDay2::regCtrlConditionHelper();
  CCMonth::regCtrlConditionHelper();
  CCMonthDay::regCtrlConditionHelper();
  CCWeekDayEx::regCtrlConditionHelper();
  CCMonthDayEx::regCtrlConditionHelper();
  CCIndicatorSV::regCtrlConditionHelper();
  CCIndicatorSP::regCtrlConditionHelper();
  CCIndicatorSP2::regCtrlConditionHelper();
  CCIndicatorDP::regCtrlConditionHelper();
  CCCtrlConditionID::regCtrlConditionHelper();
  CCWaitToStart::regCtrlConditionHelper();
}

CR2END