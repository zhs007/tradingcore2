#include <tradingcore2/exchange.h>
#include <tradingcore2/indicatormgr.h>
#include <tradingcore2/strategy/normal.h>
#include <tradingcore2/strategymgr.h>
#include <tradingcore2/wallet.h>

CR2BEGIN

void StrategyNormal::regStrategy() {
  StrategyMgr::getSingleton()->regStrategy(
      "normal", [](Wallet& wallet, Exchange& exchange) {
        return new StrategyNormal(wallet, exchange);
      });
}

CR2END