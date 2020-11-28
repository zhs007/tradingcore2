#include <tradingcore2/exchange.h>
#include <tradingcore2/indicatormgr.h>
#include <tradingcore2/strategy/aip.h>
#include <tradingcore2/strategymgr.h>
#include <tradingcore2/utils.h>
#include <tradingcore2/wallet.h>

CR2BEGIN

// void StrategyAIP::onTimeStamp(bool issim, TimeStamp ts, int index) {
//   if (this->m_type == TT_WEEK) {
//     tm ctm;
//     timestamp2timeUTC(ts, ctm);

//     if (ctm.tm_wday == this->m_timeParam) {
//       this->m_wallet.deposit(this->m_money, ts);
//       this->m_volume += this->m_wallet.buyAssets(this->m_assetsName.c_str(),
//                                                  this->m_money, ts);
//     }
//   } else if (this->m_type == TT_MONTH) {
//     tm ctm;
//     timestamp2timeUTC(ts, ctm);

//     if (ctm.tm_mday == this->m_timeParam) {
//       this->m_wallet.deposit(this->m_money, ts);
//       this->m_volume += this->m_wallet.buyAssets(this->m_assetsName.c_str(),
//                                                  this->m_money, ts);
//     }
//   }
// }

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