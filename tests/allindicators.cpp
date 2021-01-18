#include <gtest/gtest.h>
#include <tradingcore2/allindicators.h>
#include <tradingcore2/indicatormgr.h>

TEST(AllIndicators, newWithAvgTimes) {
  // tr2::regAllIndicators();

  auto mgr = tr2::IndicatorMgr::getSingleton();

  EXPECT_TRUE(mgr->newIndicator("ema.5", "110022") != NULL);
  EXPECT_TRUE(mgr->newIndicator("roc.5", "110022") != NULL);
  EXPECT_TRUE(mgr->newIndicator("rsi.5", "110022") != NULL);
  EXPECT_TRUE(mgr->newIndicator("sma.5", "110022") != NULL);
  EXPECT_TRUE(mgr->newIndicator("smma.5", "110022") != NULL);
  EXPECT_TRUE(mgr->newIndicator("wma.5", "110022") != NULL);
  EXPECT_TRUE(mgr->newIndicator("ta-sma.5", "110022") != NULL);
  EXPECT_TRUE(mgr->newIndicator("ta-ema.5", "110022") != NULL);
  EXPECT_TRUE(mgr->newIndicator("ta-wma.5", "110022") != NULL);
  EXPECT_TRUE(mgr->newIndicator("ta-dema.5", "110022") != NULL);
  EXPECT_TRUE(mgr->newIndicator("ta-tema.5", "110022") != NULL);
  EXPECT_TRUE(mgr->newIndicator("ta-trima.5", "110022") != NULL);
  EXPECT_TRUE(mgr->newIndicator("ta-kama.5", "110022") != NULL);
  EXPECT_TRUE(mgr->newIndicator("ta-mama.5", "110022") != NULL);
  EXPECT_TRUE(mgr->newIndicator("ta-t3.5", "110022") != NULL);
}