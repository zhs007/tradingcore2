#include <tradingcore2/allindicators.h>
#include <tradingcore2/indicator/ema.h>
#include <tradingcore2/indicator/roc.h>
#include <tradingcore2/indicator/rsi.h>
#include <tradingcore2/indicator/sma.h>
#include <tradingcore2/indicator/smma.h>
#include <tradingcore2/indicator/ta_ma.h>
#include <tradingcore2/indicator/wma.h>
#include <tradingcore2/indicatormgr.h>

CR2BEGIN

void regAllIndicators() {
  auto mgr = IndicatorMgr::getSingleton();

  mgr->regIndicatorWithAvgTimes("ema", IndicatorEMA::newIndicator);
  mgr->regIndicatorWithAvgTimes("roc", IndicatorROC::newIndicator);
  mgr->regIndicatorWithAvgTimes("rsi", IndicatorRSI::newIndicator);
  mgr->regIndicatorWithAvgTimes("sma", IndicatorSMA::newIndicator);
  mgr->regIndicatorWithAvgTimes("smma", IndicatorSMMA::newIndicator);
  mgr->regIndicatorWithAvgTimes("wma", IndicatorWMA::newIndicator);
  mgr->regIndicatorWithAvgTimes("ta_ma", IndicatorTA_MA::newIndicator);
}

CR2END