#include <gtest/gtest.h>
#include <stdio.h>
#include <tradingcore2/exchange/cnfund.h>
#include <tradingcore2/indicator/ema.h>

class EMATest : public testing::Test {
 protected:
  virtual void SetUp() override { cnfund.loadFundValue("../data/cnfund/110022.csv"); }

  virtual void TearDown() override {}

  tr2::CNFundExchange cnfund;
};

TEST_F(EMATest, ema_2) {
  tr2::IndicatorEMA* pEMA = new tr2::IndicatorEMA(2);

  auto isok = pEMA->build(cnfund, "110022", 0, 2280);
  EXPECT_TRUE(isok);

  EXPECT_EQ(pEMA->getLength(), 2280);

  EXPECT_NEAR(pEMA->getSingleValue(0)->value, 1.0, 0.00005);
  EXPECT_NEAR(pEMA->getSingleValue(1)->value,
              (2 * 1.0009 + pEMA->getSingleValue(0)->value) / 3, 0.00005);
  EXPECT_NEAR(pEMA->getSingleValue(2)->value,
              (2 * 1.006 + pEMA->getSingleValue(1)->value) / 3, 0.00005);
  EXPECT_NEAR(pEMA->getSingleValue(2278)->value,
              (2 * 2.911 + pEMA->getSingleValue(2277)->value) / 3, 0.00005);
  EXPECT_NEAR(pEMA->getSingleValue(2279)->value,
              (2 * 2.915 + pEMA->getSingleValue(2278)->value) / 3, 0.00005);
}

TEST_F(EMATest, ema_3) {
  tr2::IndicatorEMA* pEMA = new tr2::IndicatorEMA(3);

  auto isok = pEMA->build(cnfund, "110022", 0, 2280);
  EXPECT_TRUE(isok);

  EXPECT_EQ(pEMA->getLength(), 2280);

  EXPECT_NEAR(pEMA->getSingleValue(0)->value, 1.0, 0.00005);
  EXPECT_NEAR(pEMA->getSingleValue(1)->value,
              (2 * 1.0009 + 2 * pEMA->getSingleValue(0)->value) / 4, 0.00005);
  EXPECT_NEAR(pEMA->getSingleValue(2)->value,
              (2 * 1.006 + 2 * pEMA->getSingleValue(1)->value) / 4, 0.00005);
  EXPECT_NEAR(pEMA->getSingleValue(3)->value,
              (2 * 1.006 + 2 * pEMA->getSingleValue(2)->value) / 4, 0.00005);
  EXPECT_NEAR(pEMA->getSingleValue(2278)->value,
              (2 * 2.911 + 2 * pEMA->getSingleValue(2277)->value) / 4, 0.00005);
  EXPECT_NEAR(pEMA->getSingleValue(2279)->value,
              (2 * 2.915 + 2 * pEMA->getSingleValue(2278)->value) / 4, 0.00005);
}