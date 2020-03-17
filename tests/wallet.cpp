#include <gtest/gtest.h>
#include <stdio.h>
#include <tradingcore2/exchange/cnfund.h>
#include <tradingcore2/utils.h>
#include <tradingcore2/wallet.h>

class WalletTest : public testing::Test {
 protected:
  virtual void SetUp() override { cnfund.loadFundValue("../data/110022.csv"); }

  virtual void TearDown() override {}

  tr2::CNFundExchange cnfund;
};

TEST_F(WalletTest, depositandwithdraw) {
  auto pWallet = new tr2::Wallet(cnfund);

  pWallet->deposit(10000, tr2::str2timestampUTC("20100820", "%Y%m%d"));
  EXPECT_EQ(pWallet->getMoney(), 10000);

  pWallet->deposit(100, tr2::str2timestampUTC("20100820", "%Y%m%d"));
  EXPECT_EQ(pWallet->getMoney(), 10100);

  pWallet->withdraw(1000, tr2::str2timestampUTC("20100820", "%Y%m%d"));
  EXPECT_EQ(pWallet->getMoney(), 10100 - 1000);

  pWallet->withdraw(300, tr2::str2timestampUTC("20100820", "%Y%m%d"));
  EXPECT_EQ(pWallet->getMoney(), 10100 - 1000 - 300);

  pWallet->deposit(300, tr2::str2timestampUTC("20100820", "%Y%m%d"));
  EXPECT_EQ(pWallet->getMoney(), 10100 - 1000);

  pWallet->withdraw(9100, tr2::str2timestampUTC("20100820", "%Y%m%d"));
  EXPECT_EQ(pWallet->getMoney(), 0);

  pWallet->withdraw(1, tr2::str2timestampUTC("20100820", "%Y%m%d"));
  EXPECT_EQ(pWallet->getMoney(), 0);

  delete pWallet;
}

TEST_F(WalletTest, trade) {
  auto pWallet = new tr2::Wallet(cnfund);

  pWallet->deposit(10000, tr2::str2timestampUTC("20100820", "%Y%m%d"));
  EXPECT_EQ(pWallet->getMoney(), 10000);

  pWallet->buyAssets("110022", 100,
                     tr2::str2timestampUTC("20100820", "%Y%m%d"));
  EXPECT_EQ(pWallet->getMoney(), 9900);

  auto assets = pWallet->getAssets("110022");
  EXPECT_EQ(assets.inPrice, 1);
  EXPECT_EQ(assets.volume, 100);
  EXPECT_EQ(assets.blocks.size(), 1);

  pWallet->sellAssets("110022", 100,
                      tr2::str2timestampUTC("20100827", "%Y%m%d"));
  EXPECT_NEAR(pWallet->getMoney(), 10000.09, 0.00004);

  assets = pWallet->getAssets("110022");
  EXPECT_EQ(assets.inPrice, 0);
  EXPECT_EQ(assets.volume, 0);
  EXPECT_EQ(assets.blocks.size(), 0);

  delete pWallet;
}

TEST_F(WalletTest, trade2) {
  auto pWallet = new tr2::Wallet(cnfund);

  pWallet->deposit(10000, tr2::str2timestampUTC("20100820", "%Y%m%d"));
  EXPECT_EQ(pWallet->getMoney(), 10000);

  pWallet->buyAssets("110022", 1000,
                     tr2::str2timestampUTC("20100820", "%Y%m%d"));
  EXPECT_EQ(pWallet->getMoney(), 9000);

  auto assets = pWallet->getAssets("110022");
  EXPECT_EQ(assets.inPrice, 1);
  EXPECT_EQ(assets.volume, 1000);
  EXPECT_EQ(assets.blocks.size(), 1);

  pWallet->sellAssets("110022", 100,
                      tr2::str2timestampUTC("20100827", "%Y%m%d"));
  EXPECT_NEAR(pWallet->getMoney(), 9100.09, 0.00004);

  assets = pWallet->getAssets("110022");
  EXPECT_EQ(assets.inPrice, 1);
  EXPECT_EQ(assets.volume, 900);
  EXPECT_EQ(assets.blocks.size(), 1);

  pWallet->buyAssets("110022", 1010,
                     tr2::str2timestampUTC("20101022", "%Y%m%d"));
  EXPECT_EQ(pWallet->getMoney(), 9100.09 - 1010);

  assets = pWallet->getAssets("110022");
  EXPECT_EQ(assets.inPrice, (1 * 900 + 1.01 * 1000) / 1900);
  EXPECT_EQ(assets.volume, 1900);
  EXPECT_EQ(assets.blocks.size(), 2);

  pWallet->sellAssets("110022", 1800,
                      tr2::str2timestampUTC("20200212", "%Y%m%d"));
  EXPECT_NEAR(pWallet->getMoney(), 9100.09 - 1010 + 1800 * 2.915, 0.00004);

  assets = pWallet->getAssets("110022");
  EXPECT_EQ(assets.inPrice, (1 * 900 + 1.01 * 1000) / 1900);
  EXPECT_EQ(assets.volume, 100);
  EXPECT_EQ(assets.blocks.size(), 1);

  delete pWallet;
}