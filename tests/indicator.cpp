#include <gtest/gtest.h>
#include <tradingcore2/tradingcore2.h>

class IndicatorTest : public testing::Test {
 protected:
  virtual void SetUp() override {
    cnfund = tr2::ExchangeMgr::getSingleton()->newExchange("cnfund");
    // tr2::regAllIndicators();

    // cnfund.loadFundValue("../data/cnfund/110022.csv");
  }

  virtual void TearDown() override {
    tr2::ExchangeMgr::getSingleton()->deleteExchange(cnfund);
  }

  tr2::Exchange* cnfund;
};

TEST_F(IndicatorTest, ema5) {
  auto mgr = tr2::IndicatorMgr::getSingleton();

  auto pEMA = mgr->newIndicator("ema.5", "110022");
  pEMA->build(*cnfund, "110022", 0, cnfund->getDataLength("110022"));

  pEMA->saveCSV("../output/test.ema5.csv");
}

TEST_F(IndicatorTest, sma1) {
  auto mgr = tr2::IndicatorMgr::getSingleton();

  auto pSMA = mgr->newIndicator("sma.1", "110022");
  pSMA->build(*cnfund, "110022", 0, cnfund->getDataLength("110022"));

  pSMA->saveCSV("../output/test.sma1.csv");
}

TEST_F(IndicatorTest, smma10) {
  auto mgr = tr2::IndicatorMgr::getSingleton();

  auto pEMA = mgr->newIndicator("smma.10", "110022");
  pEMA->build(*cnfund, "110022", 0, cnfund->getDataLength("110022"));

  pEMA->saveCSV("../output/test.smma10.csv");
}

TEST_F(IndicatorTest, rsi7) {
  auto mgr = tr2::IndicatorMgr::getSingleton();

  auto pEMA = mgr->newIndicator("rsi.7", "110022");
  pEMA->build(*cnfund, "110022", 0, cnfund->getDataLength("110022"));

  pEMA->saveCSV("../output/test.rsi.csv");
}

TEST_F(IndicatorTest, parseIndicatorParams) {
  tr2::IndicatorParams params;

  bool isok = tr2::parseIndicatorParams(params, "ema.5", "jqdata.000001_OF|1d");
  EXPECT_EQ(isok, true);
  EXPECT_EQ(params.name, "ema");
  EXPECT_EQ(params.avgtimes, 5);
  EXPECT_EQ(params.b2type, tr2::IB2T_NONE);

  isok = tr2::parseIndicatorParams(params, "ema.29>day/1d/5m/255",
                                   "jqdata.000001_OF|1m");
  EXPECT_EQ(isok, true);
  EXPECT_EQ(params.name, "ema");
  EXPECT_EQ(params.avgtimes, 29);
  EXPECT_EQ(params.b2type, tr2::IB2T_DAY);
  EXPECT_EQ(params.b2OffTime, 255);
  EXPECT_EQ(params.assetsNames.size(), 2);
  EXPECT_EQ(params.assetsNames[0], "000001_OF|1d");
  EXPECT_EQ(params.assetsNames[1], "000001_OF|5m");

  isok =
      tr2::parseIndicatorParams(params, "ema.29>day/1d/5m/255", "000001_OF|1m");
  EXPECT_EQ(isok, true);
  EXPECT_EQ(params.name, "ema");
  EXPECT_EQ(params.avgtimes, 29);
  EXPECT_EQ(params.b2type, tr2::IB2T_DAY);
  EXPECT_EQ(params.b2OffTime, 255);
  EXPECT_EQ(params.assetsNames.size(), 2);
  EXPECT_EQ(params.assetsNames[0], "000001_OF|1d");
  EXPECT_EQ(params.assetsNames[1], "000001_OF|5m");

  isok = tr2::parseIndicatorParams(params, "ema.29>day/1d/5m/255", "000001_OF");
  EXPECT_EQ(isok, true);
  EXPECT_EQ(params.name, "ema");
  EXPECT_EQ(params.avgtimes, 29);
  EXPECT_EQ(params.b2type, tr2::IB2T_DAY);
  EXPECT_EQ(params.b2OffTime, 255);
  EXPECT_EQ(params.assetsNames.size(), 2);
  EXPECT_EQ(params.assetsNames[0], "000001_OF|1d");
  EXPECT_EQ(params.assetsNames[1], "000001_OF|5m");

  isok = tr2::parseIndicatorParams(params, "ema.abc", "jqdata.000001_OF|1d");
  EXPECT_EQ(isok, false);

  isok = tr2::parseIndicatorParams(params, "ema.abc>day/1d/5m/255",
                                   "jqdata.000001_OF|1d");
  EXPECT_EQ(isok, false);

  isok = tr2::parseIndicatorParams(params, "ema.5>day2/1d/5m/255",
                                   "jqdata.000001_OF|1d");
  EXPECT_EQ(isok, false);

  isok = tr2::parseIndicatorParams(params, "ema.5>day/1d/5m/abc",
                                   "jqdata.000001_OF|1d");
  EXPECT_EQ(isok, false);
}