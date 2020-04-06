#include <gtest/gtest.h>
#include <tradingcore2/allindicators.h>
#include <tradingcore2/exchange/cnfund.h>
#include <tradingcore2/indicator.h>
#include <tradingcore2/indicatormgr.h>

class IndicatorTest : public testing::Test {
 protected:
  virtual void SetUp() override {
    // tr2::regAllIndicators();

    cnfund.loadFundValue("../data/cnfund/110022.csv");
  }

  virtual void TearDown() override {}

  tr2::CNFundExchange cnfund;
};

TEST_F(IndicatorTest, ema5) {
  auto mgr = tr2::IndicatorMgr::getSingleton();

  auto pEMA = mgr->newIndicator("ema", 5);
  pEMA->build(cnfund, "110022", 0, cnfund.getDataLength("110022"));

  pEMA->saveCSV("../data/test.ema5.csv");
}

TEST_F(IndicatorTest, sma1) {
  auto mgr = tr2::IndicatorMgr::getSingleton();

  auto pSMA = mgr->newIndicator("sma", 1);
  pSMA->build(cnfund, "110022", 0, cnfund.getDataLength("110022"));

  pSMA->saveCSV("../data/test.sma1.csv");
}

TEST_F(IndicatorTest, smma10) {
  auto mgr = tr2::IndicatorMgr::getSingleton();

  auto pEMA = mgr->newIndicator("smma", 10);
  pEMA->build(cnfund, "110022", 0, cnfund.getDataLength("110022"));

  pEMA->saveCSV("../data/test.smma10.csv");
}

TEST_F(IndicatorTest, rsi7) {
  auto mgr = tr2::IndicatorMgr::getSingleton();

  auto pEMA = mgr->newIndicator("rsi", 7);
  pEMA->build(cnfund, "110022", 0, cnfund.getDataLength("110022"));

  pEMA->saveCSV("../data/test.rsi.csv");
}