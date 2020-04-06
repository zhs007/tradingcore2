#include <tradingcore2/allexchanges.h>
#include <tradingcore2/exchange/cnfund.h>
#include <tradingcore2/exchangemgr.h>

CR2BEGIN

void regAllExchanges() {
  auto mgr = ExchangeMgr::getSingleton();

  mgr->regNewExchange(CNFundTypeName, newCNFund);
}

CR2END