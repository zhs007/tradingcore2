#include <tradingcore2/allexchanges.h>
#include <tradingcore2/exchange/bitmex.h>
#include <tradingcore2/exchange/cnfund.h>
#include <tradingcore2/exchange/jqdata.h>
#include <tradingcore2/exchange/jrj.h>
#include <tradingcore2/exchange/trdb2cnfunds.h>
#include <tradingcore2/exchangemgr.h>

CR2BEGIN

void regAllExchanges() {
  auto mgr = ExchangeMgr::getSingleton();

  CNFundExchange::regExchange();
  TrDB2CNFundsExchange::regExchange();
  JRJExchange::regExchange();
  JQDataExchange::regExchange();
  BitMexExchange::regExchange();
}

CR2END