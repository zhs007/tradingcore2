#include <gtest/gtest.h>
#include <stdio.h>
#include <tradingcore2/exchange/cnfund.h>
#include <tradingcore2/indicator/smma.h>

class SMMATest : public testing::Test {
 protected:
  virtual void SetUp() override { cnfund.loadFundValue("../data/110022.csv"); }

  virtual void TearDown() override {}

  tr2::CNFundExchange cnfund;
};

TEST_F(SMMATest, smma_2) {
  tr2::IndicatorSMMA* pSMMA = new tr2::IndicatorSMMA(2);

  auto isok = pSMMA->build(cnfund, "110022", 0, 2280);
  EXPECT_TRUE(isok);

  EXPECT_EQ(pSMMA->getLength(), 2280);

  EXPECT_NEAR(pSMMA->getSingleValue(0)->value, 1.0, 0.00005);
  EXPECT_NEAR(pSMMA->getSingleValue(1)->value,
              (pSMMA->getSingleValue(0)->value + 1.0009) / 2, 0.00005);
  EXPECT_NEAR(pSMMA->getSingleValue(2)->value,
              (pSMMA->getSingleValue(1)->value + 1.006) / 2, 0.00005);
  EXPECT_NEAR(pSMMA->getSingleValue(3)->value,
              (pSMMA->getSingleValue(2)->value + 1.006) / 2, 0.00005);
  EXPECT_NEAR(pSMMA->getSingleValue(4)->value,
              (pSMMA->getSingleValue(3)->value + 1.004) / 2, 0.00005);
  EXPECT_NEAR(pSMMA->getSingleValue(2278)->value,
              (pSMMA->getSingleValue(2277)->value + 2.911) / 2, 0.00005);
  EXPECT_NEAR(pSMMA->getSingleValue(2279)->value,
              (pSMMA->getSingleValue(2278)->value + 2.915) / 2, 0.00005);
}

TEST_F(SMMATest, smma_3) {
  tr2::IndicatorSMMA* pSMMA = new tr2::IndicatorSMMA(3);

  auto isok = pSMMA->build(cnfund, "110022", 0, 2280);
  EXPECT_TRUE(isok);

  EXPECT_EQ(pSMMA->getLength(), 2280);

  EXPECT_NEAR(pSMMA->getSingleValue(0)->value, 1.0, 0.00005);
  EXPECT_NEAR(pSMMA->getSingleValue(1)->value,
              (2 * pSMMA->getSingleValue(0)->value + 1.0009) / 3, 0.00005);
  EXPECT_NEAR(pSMMA->getSingleValue(2)->value,
              (2 * pSMMA->getSingleValue(1)->value + 1.006) / 3, 0.00005);
  EXPECT_NEAR(pSMMA->getSingleValue(3)->value,
              (2 * pSMMA->getSingleValue(2)->value + 1.006) / 3, 0.00005);
  EXPECT_NEAR(pSMMA->getSingleValue(2278)->value,
              (2 * pSMMA->getSingleValue(2277)->value + 2.911) / 3, 0.00005);
  EXPECT_NEAR(pSMMA->getSingleValue(2279)->value,
              (2 * pSMMA->getSingleValue(2278)->value + 2.915) / 3, 0.00005);
}