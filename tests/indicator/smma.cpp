#include <gtest/gtest.h>
#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

class SMMATest : public testing::Test {
 protected:
  virtual void SetUp() override {
    cnfund = tr2::ExchangeMgr::getSingleton()->newExchange("cnfund");
  }

  virtual void TearDown() override {}

  tr2::Exchange* cnfund;
};

TEST_F(SMMATest, smma_2) {
  tr2::IndicatorSMMA* pSMMA =
      (tr2::IndicatorSMMA*)tr2::IndicatorSMMA::newIndicator("smma.2");

  auto isok = pSMMA->build(*cnfund, "110022", 0, 2280);
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
  tr2::IndicatorSMMA* pSMMA =
      (tr2::IndicatorSMMA*)tr2::IndicatorSMMA::newIndicator("smma.3");

  auto isok = pSMMA->build(*cnfund, "110022", 0, 2280);
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