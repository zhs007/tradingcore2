#include <gtest/gtest.h>
#include <tradingcore2/allindicators.h>
#include <tradingcore2/indicatormgr.h>

TEST(AllIndicators, newWithAvgTimes) {
  // tr2::regAllIndicators();

  auto mgr = tr2::IndicatorMgr::getSingleton();

  EXPECT_TRUE(mgr->newIndicator("ema", 5) != NULL);
  EXPECT_TRUE(mgr->newIndicator("roc", 5) != NULL);
  EXPECT_TRUE(mgr->newIndicator("rsi", 5) != NULL);
  EXPECT_TRUE(mgr->newIndicator("sma", 5) != NULL);
  EXPECT_TRUE(mgr->newIndicator("smma", 5) != NULL);
  EXPECT_TRUE(mgr->newIndicator("wma", 5) != NULL);
}