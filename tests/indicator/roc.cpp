#include <gtest/gtest.h>
#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

class ROCTest : public testing::Test {
 protected:
  virtual void SetUp() override {
    cnfund = tr2::ExchangeMgr::getSingleton()->newExchange("cnfund");
  }

  virtual void TearDown() override {}

  tr2::Exchange* cnfund;
};

TEST_F(ROCTest, roc_1) {
  tr2::IndicatorROC* pROC = new tr2::IndicatorROC(1);

  auto isok = pROC->build(*cnfund, "110022", 0, 2280);
  EXPECT_TRUE(isok);

  EXPECT_EQ(pROC->getLength(), 2280);

  EXPECT_NEAR(pROC->getSingleValue(0)->value, 0.0, 0.00005);
  EXPECT_NEAR(pROC->getSingleValue(1)->value, (1.0009 - 1.0) / 1.0, 0.00005);
  EXPECT_NEAR(pROC->getSingleValue(2)->value, (1.006 - 1.0009) / 1.0009,
              0.00005);
  EXPECT_NEAR(pROC->getSingleValue(2278)->value, (2.911 - 2.848) / 2.848,
              0.00005);
  EXPECT_NEAR(pROC->getSingleValue(2279)->value, (2.915 - 2.911) / 2.911,
              0.00005);
}

TEST_F(ROCTest, roc_2) {
  tr2::IndicatorROC* pROC = new tr2::IndicatorROC(2);

  auto isok = pROC->build(*cnfund, "110022", 0, 2280);
  EXPECT_TRUE(isok);

  EXPECT_EQ(pROC->getLength(), 2280);

  EXPECT_NEAR(pROC->getSingleValue(0)->value, 0.0, 0.00005);
  EXPECT_NEAR(pROC->getSingleValue(1)->value, 0.0, 0.00005);
  EXPECT_NEAR(pROC->getSingleValue(2)->value, (1.006 - 1.0) / 1.0, 0.00005);
  EXPECT_NEAR(pROC->getSingleValue(3)->value, (1.006 - 1.0009) / 1.0009,
              0.00005);
  EXPECT_NEAR(pROC->getSingleValue(2278)->value, (2.911 - 2.838) / 2.838,
              0.00005);
  EXPECT_NEAR(pROC->getSingleValue(2279)->value, (2.915 - 2.848) / 2.848,
              0.00005);
}