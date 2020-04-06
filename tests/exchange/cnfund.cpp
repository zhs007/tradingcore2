#include <gtest/gtest.h>
#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

class CNFundTest : public testing::Test {
 protected:
  virtual void SetUp() override {
    cnfund =
        (tr2::CNFundExchange*)tr2::ExchangeMgr::getSingleton()->getExchange(
            "cnfund");
    // cnfund.loadFundValue("../data/cnfund/110022.csv");
  }

  virtual void TearDown() override {}

  tr2::CNFundExchange* cnfund;
};

TEST_F(CNFundTest, loadfv) {
  auto fv = cnfund->getFundValue("110022");
  EXPECT_TRUE(fv != NULL);

  EXPECT_STREQ(fv->code.c_str(), "110022");

  EXPECT_EQ(fv->data.size(), 2280);

  EXPECT_EQ(fv->data[0].value, 1.0);
  EXPECT_EQ(fv->data[0].totalValue, 1.0);
  EXPECT_EQ(fv->data[0].ts, 1282262400);

  EXPECT_EQ(fv->data[1].value, 1.0009);
  EXPECT_EQ(fv->data[1].totalValue, 1.0009);
  EXPECT_EQ(fv->data[1].ts, 1282867200);

  EXPECT_EQ(fv->data[2279].value, 2.915);
  EXPECT_EQ(fv->data[2279].totalValue, 2.915);
  EXPECT_EQ(fv->data[2279].ts, 1581465600);
}

TEST_F(CNFundTest, getnode) {
  auto fv = cnfund->getFundValue("110022");
  EXPECT_TRUE(fv != NULL);

  auto n = fv->getNode(1282262400);
  EXPECT_TRUE(n != NULL);

  EXPECT_EQ(n->value, 1.0);
  EXPECT_EQ(n->totalValue, 1.0);
  EXPECT_EQ(n->ts, 1282262400);

  n = fv->getNode(1282262400 - 1);
  EXPECT_TRUE(n != NULL);

  EXPECT_EQ(n->value, 1.0);
  EXPECT_EQ(n->totalValue, 1.0);
  EXPECT_EQ(n->ts, 1282262400);

  n = fv->getNode(1282262400 + 1);
  EXPECT_TRUE(n != NULL);

  EXPECT_EQ(n->value, 1.0009);
  EXPECT_EQ(n->totalValue, 1.0009);
  EXPECT_EQ(n->ts, 1282867200);

  n = fv->getNode(1282867200 - 1);
  EXPECT_TRUE(n != NULL);

  EXPECT_EQ(n->value, 1.0009);
  EXPECT_EQ(n->totalValue, 1.0009);
  EXPECT_EQ(n->ts, 1282867200);

  n = fv->getNode(1282867200);
  EXPECT_TRUE(n != NULL);

  EXPECT_EQ(n->value, 1.0009);
  EXPECT_EQ(n->totalValue, 1.0009);
  EXPECT_EQ(n->ts, 1282867200);

  n = fv->getNode(1282867200 + 1);
  EXPECT_TRUE(n != NULL);

  EXPECT_EQ(n->value, 1.006);
  EXPECT_EQ(n->totalValue, 1.006);
  EXPECT_EQ(n->ts, 1283472000);

  n = fv->getNode(1581465600 - 1);
  EXPECT_TRUE(n != NULL);

  EXPECT_EQ(n->value, 2.915);
  EXPECT_EQ(n->totalValue, 2.915);
  EXPECT_EQ(n->ts, 1581465600);

  n = fv->getNode(1581465600);
  EXPECT_TRUE(n != NULL);

  EXPECT_EQ(n->value, 2.915);
  EXPECT_EQ(n->totalValue, 2.915);
  EXPECT_EQ(n->ts, 1581465600);

  n = fv->getNode(1581465600 + 1);
  EXPECT_TRUE(n == NULL);
}