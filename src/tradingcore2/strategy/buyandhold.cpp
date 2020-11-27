#include <tradingcore2/exchange.h>
#include <tradingcore2/indicatormgr.h>
#include <tradingcore2/strategy/buyandhold.h>
#include <tradingcore2/wallet.h>

CR2BEGIN

// void StrategyBAH::onTimeStamp(bool issim, TimeStamp ts, int index) {
//   if (index == 0) {
//     this->m_wallet.deposit(this->m_money, ts);
//     this->m_volume =
//         this->m_wallet.buyAssets(this->m_assetsName.c_str(), this->m_money,
//         ts);
//   }
// }

void StrategyBAH::onBuy(bool issim, TimeStamp ts, Money money, Volume volume,
                        Money fee) {
  Strategy::onBuy(issim, ts, money, volume, fee);

  this->m_wallet.deposit(money, ts);
}

CR2END