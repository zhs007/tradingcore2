#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

void runBuyAndHold() {
  tr2::CNFundExchange cnfund;

  cnfund.loadFundValue("../data/110022.csv");
  cnfund.buildTimeStampList();

  auto pWallet = new tr2::Wallet(cnfund);

  pWallet->deposit(10000, tr2::str2timestampUTC("20100820", "%Y%m%d"));

  tr2::StrategyBAH* bah = new tr2::StrategyBAH(*pWallet, cnfund);

  bah->simulateTrading();

  tr2::PNL pnl;
  pWallet->buildPNL(pnl);

  pnl.print("buy and hold 110022");
}

int main() {
  runBuyAndHold();

  return 0;
}