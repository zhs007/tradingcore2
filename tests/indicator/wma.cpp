#include <gtest/gtest.h>
#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

class WMATest : public testing::Test {
 protected:
  virtual void SetUp() override {
    cnfund = tr2::ExchangeMgr::getSingleton()->newExchange("cnfund");
  }

  virtual void TearDown() override {}

  tr2::Exchange* cnfund;
};

TEST_F(WMATest, wma_2) {
  tr2::IndicatorWMA* pWMA =
      (tr2::IndicatorWMA*)tr2::IndicatorWMA::newIndicator("wma.2", "110022");

  auto isok = pWMA->build(*cnfund, "110022", 0, 2280);
  EXPECT_TRUE(isok);

  EXPECT_EQ(pWMA->getLength(), 2280);

  EXPECT_NEAR(pWMA->getSingleValue(0)->value, 1.0, 0.00005);
  EXPECT_NEAR(pWMA->getSingleValue(1)->value, (2 * 1.0009 + 1.0) / 3, 0.00005);
  EXPECT_NEAR(pWMA->getSingleValue(2)->value, (2 * 1.006 + 1.0009) / 3,
              0.00005);
  EXPECT_NEAR(pWMA->getSingleValue(2278)->value, (2 * 2.911 + 2.848) / 3,
              0.00005);
  EXPECT_NEAR(pWMA->getSingleValue(2279)->value, (2 * 2.915 + 2.911) / 3,
              0.00005);
}

TEST_F(WMATest, wma_3) {
  tr2::IndicatorWMA* pWMA =
      (tr2::IndicatorWMA*)tr2::IndicatorWMA::newIndicator("wma.3", "110022");

  auto isok = pWMA->build(*cnfund, "110022", 0, 2280);
  EXPECT_TRUE(isok);

  EXPECT_EQ(pWMA->getLength(), 2280);

  EXPECT_NEAR(pWMA->getSingleValue(0)->value, 1.0, 0.00005);
  EXPECT_NEAR(pWMA->getSingleValue(1)->value, (3 * 1.0009 + 2 * 1.0 + 1.0) / 6,
              0.00005);
  EXPECT_NEAR(pWMA->getSingleValue(2)->value,
              (3 * 1.006 + 2 * 1.0009 + 1.0) / 6, 0.00005);
  EXPECT_NEAR(pWMA->getSingleValue(3)->value,
              (3 * 1.006 + 2 * 1.006 + 1.0009) / 6, 0.00005);
  EXPECT_NEAR(pWMA->getSingleValue(2278)->value,
              (3 * 2.911 + 2 * 2.848 + 2.838) / 6, 0.00005);
  EXPECT_NEAR(pWMA->getSingleValue(2279)->value,
              (3 * 2.915 + 2 * 2.911 + 2.848) / 6, 0.00005);
}