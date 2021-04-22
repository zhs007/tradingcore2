#include <tradingcore2/allindicators.h>
#include <tradingcore2/indicator/ema.h>
#include <tradingcore2/indicator/roc.h>
#include <tradingcore2/indicator/rsi.h>
#include <tradingcore2/indicator/sma.h>
#include <tradingcore2/indicator/smma.h>
#include <tradingcore2/indicator/ta_ma.h>
#include <tradingcore2/indicator/ta_rsi.h>
#include <tradingcore2/indicator/wma.h>
#include <tradingcore2/indicatormgr.h>

CR2BEGIN

void regAllIndicators() {
  auto mgr = IndicatorMgr::getSingleton();

  mgr->regIndicator("ema", IndicatorEMA::newIndicator, IndicatorEMA::isMine);
  mgr->regIndicator("roc", IndicatorROC::newIndicator, IndicatorROC::isMine);
  mgr->regIndicator("rsi", IndicatorRSI::newIndicator, IndicatorRSI::isMine);
  mgr->regIndicator("sma", IndicatorSMA::newIndicator, IndicatorSMA::isMine);
  mgr->regIndicator("smma", IndicatorSMMA::newIndicator, IndicatorSMMA::isMine);
  mgr->regIndicator("wma", IndicatorWMA::newIndicator, IndicatorWMA::isMine);
  mgr->regIndicator("ta-ma", IndicatorTA_MA::newIndicator,
                    IndicatorTA_MA::isMine);
  mgr->regIndicator("ta-rsi", IndicatorTA_RSI::newIndicator,
                    IndicatorTA_RSI::isMine);
}

CR2END