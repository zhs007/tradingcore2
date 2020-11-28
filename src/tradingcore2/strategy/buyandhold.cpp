#include <tradingcore2/exchange.h>
#include <tradingcore2/indicatormgr.h>
#include <tradingcore2/strategy/buyandhold.h>
#include <tradingcore2/strategymgr.h>
#include <tradingcore2/wallet.h>

CR2BEGIN

void StrategyBAH::regStrategy() {
  StrategyMgr::getSingleton()->regStrategy(
      "bah", [](Wallet& wallet, Exchange& exchange) {
        return new StrategyBAH(wallet, exchange);
      });
}

CR2END