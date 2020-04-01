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

  pnl.saveCSV("../data/buyandhold.110022.csv", true);
}

void runSIROC() {
  tr2::CNFundExchange cnfund;

  cnfund.loadFundValue("../data/110022.csv");
  cnfund.buildTimeStampList();

  auto pWallet = new tr2::Wallet(cnfund);

  pWallet->deposit(10000, tr2::str2timestampUTC("20100820", "%Y%m%d"));

  tr2::StrategySI* si = new tr2::StrategySI(*pWallet, cnfund);
  si->init("110022", "roc", 1, -0.02, -0.015, 0.03, 0.04, 10000);
  si->setStopLess(0.10);

  si->simulateTrading();

  tr2::PNL pnl;
  pWallet->buildPNL(pnl);

  pnl.print("roc 110022");
  si->print();

  pnl.saveCSV("../data/roc.110022.csv", true);
}

void train() {
  tr2::CNFundExchange cnfund;

  cnfund.loadFundValue("../data/110022.csv");
  cnfund.buildTimeStampList();

  tr2::trainSingleIndicator2(cnfund, "110022", "roc", "../output", 10000, 2,
                             0.005, 0.015, 0.005, 0.03, 2);
  // tr2::trainSingleIndicator2(cnfund, "110022", "rsi", "../output", 10000, 5,
  // 2,
  //                            5, 2, 10, 2);
}

int main() {
  tr2::regAllIndicators();

  // runBuyAndHold();
  // runSIROC();
  train();

  return 0;
}