#include <gtest/gtest.h>
#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

class TA_MATest : public testing::Test {
 protected:
  virtual void SetUp() override {
    cnfund = tr2::ExchangeMgr::getSingleton()->newExchange("cnfund");
  }

  virtual void TearDown() override {}

  tr2::Exchange* cnfund;
};

TEST_F(TA_MATest, ta_ma_2) {
  tr2::IndicatorTA_MA* pTA_MA =
      (tr2::IndicatorTA_MA*)tr2::IndicatorTA_MA::newIndicator("ta-sma.2",
                                                              "110022");

  auto isok = pTA_MA->build(*cnfund, "110022", 0, 2280);
  EXPECT_TRUE(isok);

  EXPECT_EQ(pTA_MA->getLength(), 2280);

  EXPECT_NEAR(pTA_MA->getSingleValue(0)->value, 1.0, 0.00005);
  EXPECT_NEAR(pTA_MA->getSingleValue(1)->value, (1.0 + 1.0009) / 2, 0.00005);
  EXPECT_NEAR(pTA_MA->getSingleValue(2)->value, (1.0009 + 1.006) / 2, 0.00005);
  EXPECT_NEAR(pTA_MA->getSingleValue(2278)->value, (2.848 + 2.911) / 2,
              0.00005);
  EXPECT_NEAR(pTA_MA->getSingleValue(2279)->value, (2.911 + 2.915) / 2,
              0.00005);
}

TEST_F(TA_MATest, ta_ma_3) {
  tr2::IndicatorTA_MA* pTA_MA =
      (tr2::IndicatorTA_MA*)tr2::IndicatorTA_MA::newIndicator("ta-sma.3",
                                                              "110022");

  auto isok = pTA_MA->build(*cnfund, "110022", 0, 2280);
  EXPECT_TRUE(isok);

  EXPECT_EQ(pTA_MA->getLength(), 2280);

  EXPECT_NEAR(pTA_MA->getSingleValue(0)->value, 1.0, 0.00005);
  EXPECT_NEAR(pTA_MA->getSingleValue(1)->value, 1.0009, 0.00005);
  EXPECT_NEAR(pTA_MA->getSingleValue(2)->value, (1.0 + 1.0009 + 1.006) / 3,
              0.00005);
  EXPECT_NEAR(pTA_MA->getSingleValue(3)->value, (1.0009 + 1.006 + 1.006) / 3,
              0.00005);
  EXPECT_NEAR(pTA_MA->getSingleValue(2278)->value, (2.838 + 2.848 + 2.911) / 3,
              0.00005);
  EXPECT_NEAR(pTA_MA->getSingleValue(2279)->value, (2.848 + 2.911 + 2.915) / 3,
              0.00005);
}