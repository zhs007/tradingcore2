#include <gtest/gtest.h>
#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

class RSITest : public testing::Test {
 protected:
  virtual void SetUp() override {
    cnfund = tr2::ExchangeMgr::getSingleton()->newExchange("cnfund");
  }

  virtual void TearDown() override {}

  tr2::Exchange* cnfund;
};

TEST_F(RSITest, rsi_2) {
  tr2::IndicatorRSI* pRSI =
      (tr2::IndicatorRSI*)tr2::IndicatorRSI::newIndicator("rsi.2", "110022");

  auto isok = pRSI->build(*cnfund, "110022", 0, 2280);
  EXPECT_TRUE(isok);

  EXPECT_EQ(pRSI->getLength(), 2280);

  EXPECT_NEAR(pRSI->getSingleValue(0)->value, 100, 0.00005);

  EXPECT_NEAR(pRSI->getU(1), 0.0009, 0.00005);
  EXPECT_NEAR(pRSI->getD(1), 0, 0.00005);
  EXPECT_NEAR(pRSI->getSMMAU(1), 0.00045, 0.00005);
  EXPECT_NEAR(pRSI->getSMMAD(1), 0, 0.00005);
  EXPECT_NEAR(pRSI->getSingleValue(1)->value, 100, 0.00005);

  EXPECT_NEAR(pRSI->getU(2), 1.006 - 1.0009, 0.00005);
  EXPECT_NEAR(pRSI->getD(2), 0, 0.00005);
  EXPECT_NEAR(pRSI->getSMMAU(2), (pRSI->getSMMAU(1) + pRSI->getU(2)) / 2,
              0.00005);
  EXPECT_NEAR(pRSI->getSMMAD(2), 0, 0.00005);
  EXPECT_NEAR(pRSI->getSingleValue(2)->value, 100, 0.00005);

  EXPECT_NEAR(pRSI->getU(3), 1.006 - 1.006, 0.00005);
  EXPECT_NEAR(pRSI->getD(3), 0, 0.00005);
  EXPECT_NEAR(pRSI->getSMMAU(3), (pRSI->getSMMAU(2) + pRSI->getU(3)) / 2,
              0.00005);
  EXPECT_NEAR(pRSI->getSMMAD(3), 0, 0.00005);
  EXPECT_NEAR(pRSI->getSingleValue(3)->value, 100, 0.00005);

  EXPECT_NEAR(pRSI->getU(4), 0, 0.00005);
  EXPECT_NEAR(pRSI->getD(4), 1.006 - 1.004, 0.00005);
  EXPECT_NEAR(pRSI->getSMMAU(4), (pRSI->getSMMAU(3) + pRSI->getU(4)) / 2,
              0.00005);
  EXPECT_NEAR(pRSI->getSMMAD(4), (pRSI->getSMMAD(3) + pRSI->getD(4)) / 2,
              0.00005);
  EXPECT_NEAR(pRSI->getSingleValue(4)->value,
              100 - 100 / (1 + pRSI->getSMMAU(4) / pRSI->getSMMAD(4)), 0.00005);

  EXPECT_NEAR(pRSI->getU(2278), 2.911 - 2.848, 0.00005);
  EXPECT_NEAR(pRSI->getD(2278), 0, 0.00005);
  EXPECT_NEAR(pRSI->getSMMAU(2278),
              (pRSI->getSMMAU(2277) + pRSI->getU(2278)) / 2, 0.00005);
  EXPECT_NEAR(pRSI->getSMMAD(2278),
              (pRSI->getSMMAD(2277) + pRSI->getD(2278)) / 2, 0.00005);
  EXPECT_NEAR(pRSI->getSingleValue(2278)->value,
              100 - 100 / (1 + pRSI->getSMMAU(2278) / pRSI->getSMMAD(2278)),
              0.00005);

  EXPECT_NEAR(pRSI->getU(2279), 2.915 - 2.911, 0.00005);
  EXPECT_NEAR(pRSI->getD(2279), 0, 0.00005);
  EXPECT_NEAR(pRSI->getSMMAU(2279),
              (pRSI->getSMMAU(2278) + pRSI->getU(2279)) / 2, 0.00005);
  EXPECT_NEAR(pRSI->getSMMAD(2279),
              (pRSI->getSMMAD(2278) + pRSI->getD(2279)) / 2, 0.00005);
  EXPECT_NEAR(pRSI->getSingleValue(2279)->value,
              100 - 100 / (1 + pRSI->getSMMAU(2279) / pRSI->getSMMAD(2279)),
              0.00005);
}

TEST_F(RSITest, rsi_3) {
  tr2::IndicatorRSI* pRSI =
      (tr2::IndicatorRSI*)tr2::IndicatorRSI::newIndicator("rsi.3", "110022");

  auto isok = pRSI->build(*cnfund, "110022", 0, 2280);
  EXPECT_TRUE(isok);

  EXPECT_EQ(pRSI->getLength(), 2280);

  EXPECT_NEAR(pRSI->getSingleValue(0)->value, 100, 0.00005);

  EXPECT_NEAR(pRSI->getU(1), 0.0009, 0.00005);
  EXPECT_NEAR(pRSI->getD(1), 0, 0.00005);
  EXPECT_NEAR(pRSI->getSMMAU(1), (2 * pRSI->getSMMAU(0) + pRSI->getU(1)) / 3,
              0.00005);
  EXPECT_NEAR(pRSI->getSMMAD(1), 0, 0.00005);
  EXPECT_NEAR(pRSI->getSingleValue(1)->value, 100, 0.00005);

  EXPECT_NEAR(pRSI->getU(2), 1.006 - 1.0009, 0.00005);
  EXPECT_NEAR(pRSI->getD(2), 0, 0.00005);
  EXPECT_NEAR(pRSI->getSMMAU(2), (2 * pRSI->getSMMAU(1) + pRSI->getU(2)) / 3,
              0.00005);
  EXPECT_NEAR(pRSI->getSMMAD(2), 0, 0.00005);
  EXPECT_NEAR(pRSI->getSingleValue(2)->value, 100, 0.00005);

  EXPECT_NEAR(pRSI->getU(3), 1.006 - 1.006, 0.00005);
  EXPECT_NEAR(pRSI->getD(3), 0, 0.00005);
  EXPECT_NEAR(pRSI->getSMMAU(3), (2 * pRSI->getSMMAU(2) + pRSI->getU(3)) / 3,
              0.00005);
  EXPECT_NEAR(pRSI->getSMMAD(3), 0, 0.00005);
  EXPECT_NEAR(pRSI->getSingleValue(3)->value, 100, 0.00005);

  EXPECT_NEAR(pRSI->getU(4), 0, 0.00005);
  EXPECT_NEAR(pRSI->getD(4), 1.006 - 1.004, 0.00005);
  EXPECT_NEAR(pRSI->getSMMAU(4), (2 * pRSI->getSMMAU(3) + pRSI->getU(4)) / 3,
              0.00005);
  EXPECT_NEAR(pRSI->getSMMAD(4), (2 * pRSI->getSMMAD(3) + pRSI->getD(4)) / 3,
              0.00005);
  EXPECT_NEAR(pRSI->getSingleValue(4)->value,
              100 - 100 / (1 + pRSI->getSMMAU(4) / pRSI->getSMMAD(4)), 0.00005);

  EXPECT_NEAR(pRSI->getU(2278), 2.911 - 2.848, 0.00005);
  EXPECT_NEAR(pRSI->getD(2278), 0, 0.00005);
  EXPECT_NEAR(pRSI->getSMMAU(2278),
              (2 * pRSI->getSMMAU(2277) + pRSI->getU(2278)) / 3, 0.00005);
  EXPECT_NEAR(pRSI->getSMMAD(2278),
              (2 * pRSI->getSMMAD(2277) + pRSI->getD(2278)) / 3, 0.00005);
  EXPECT_NEAR(pRSI->getSingleValue(2278)->value,
              100 - 100 / (1 + pRSI->getSMMAU(2278) / pRSI->getSMMAD(2278)),
              0.00005);

  EXPECT_NEAR(pRSI->getU(2279), 2.915 - 2.911, 0.00005);
  EXPECT_NEAR(pRSI->getD(2279), 0, 0.00005);
  EXPECT_NEAR(pRSI->getSMMAU(2279),
              (2 * pRSI->getSMMAU(2278) + pRSI->getU(2279)) / 3, 0.00005);
  EXPECT_NEAR(pRSI->getSMMAD(2279),
              (2 * pRSI->getSMMAD(2278) + pRSI->getD(2279)) / 3, 0.00005);
  EXPECT_NEAR(pRSI->getSingleValue(2279)->value,
              100 - 100 / (1 + pRSI->getSMMAU(2279) / pRSI->getSMMAD(2279)),
              0.00005);
}