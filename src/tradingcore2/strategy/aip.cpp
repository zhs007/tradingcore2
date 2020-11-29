#include <tradingcore2/exchange.h>
#include <tradingcore2/indicatormgr.h>
#include <tradingcore2/strategy/aip.h>
#include <tradingcore2/strategymgr.h>
#include <tradingcore2/utils.h>
#include <tradingcore2/wallet.h>

CR2BEGIN

void StrategyAIP::regStrategy() {
  StrategyMgr::getSingleton()->regStrategy(
      "aip", [](Wallet& wallet, Exchange& exchange) {
        return new StrategyAIP(wallet, exchange);
      });
}

void StrategyAIP::onBuy(bool issim, TimeStamp ts, Money money, Volume volume,
                        Money fee) {
  this->m_wallet.deposit(money, ts);
  this->m_handMoney += money;

  Strategy::onBuy(issim, ts, money, volume, fee);
}

CR2END