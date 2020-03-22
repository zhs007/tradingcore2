#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

void runBuyAndHold() {
  tr2::CNFundExchange cnfund;

  cnfund.loadFundValue("../data/110022.csv");
  cnfund.buildTimeStampList();

  auto pWallet = new tr2::Wallet(cnfund);

  pWallet->deposit(10000, tr2::str2timestampUTC("20100820", "%Y%m%d"));

  tr2::StrategyBAH* bah = new tr2::StrategyBAH(*pWallet, cnfund);
  bah->init("110022", 10000);

  bah->simulateTrading();

  tr2::PNL pnl;
  pWallet->buildPNL(pnl);

  pnl.print("buy and hold 110022");
}

void runSIROC() {
  tr2::CNFundExchange cnfund;

  cnfund.loadFundValue("../data/110022.csv");
  cnfund.buildTimeStampList();

  auto pWallet = new tr2::Wallet(cnfund);

  pWallet->deposit(10000, tr2::str2timestampUTC("20100820", "%Y%m%d"));

  tr2::StrategySI* si = new tr2::StrategySI(*pWallet, cnfund);
  si->init("110022", "roc", 1, -0.02, -0.015, 0.03, 0.04, 10000);

  si->simulateTrading();

  tr2::PNL pnl;
  pWallet->buildPNL(pnl);

  pnl.print("roc 110022");
}

int main() {
  tr2::regAllIndicators();

  // runBuyAndHold();
  runSIROC();

  return 0;
}