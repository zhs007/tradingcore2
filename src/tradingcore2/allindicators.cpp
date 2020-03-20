#include <tradingcore2/allindicators.h>
#include <tradingcore2/indicator/ema.h>
#include <tradingcore2/indicator/roc.h>
#include <tradingcore2/indicator/rsi.h>
#include <tradingcore2/indicator/sma.h>
#include <tradingcore2/indicator/smma.h>
#include <tradingcore2/indicator/wma.h>
#include <tradingcore2/indicatormgr.h>

CR2BEGIN

void regAllIndicators() {
  auto mgr = IndicatorMgr::getSingleton();

  mgr->regIndicatorWithAvgTimes("ema", NewIndicatorEMA);
  mgr->regIndicatorWithAvgTimes("roc", NewIndicatorROC);
  mgr->regIndicatorWithAvgTimes("rsi", NewIndicatorRSI);
  mgr->regIndicatorWithAvgTimes("sma", NewIndicatorSMA);
  mgr->regIndicatorWithAvgTimes("smma", NewIndicatorSMMA);
  mgr->regIndicatorWithAvgTimes("wma", NewIndicatorWMA);
}

CR2END