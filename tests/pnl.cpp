#include <gtest/gtest.h>
#include <stdio.h>
#include <tradingcore2/pnl.h>

TEST(PNL, pnl_maxdrawdown1) {
  tr2::PNL pnl;

  pnl.pushData(1, 1, 1);
  pnl.pushData(2, 2, 2);
  pnl.pushData(3, 30, 30);
  pnl.pushData(4, 28, 28);
  pnl.pushData(5, 30, 30);
  pnl.pushData(6, 2, 2);
  pnl.pushData(7, 5, 5);

  pnl.calcMaxDrawdown();

  EXPECT_EQ(pnl.m_maxDrawdown, 28);
  EXPECT_EQ(pnl.m_maxDrawdownStartI, 2);
  EXPECT_EQ(pnl.m_maxDrawdownEndI, 5);
}

TEST(PNL, pnl_maxdrawdown2) {
  tr2::PNL pnl;

  pnl.pushData(1, 1, 1);
  pnl.pushData(2, 2, 2);
  pnl.pushData(3, 30, 30);
  pnl.pushData(4, 28, 28);
  pnl.pushData(5, 30, 30);
  pnl.pushData(6, 2, 2);
  pnl.pushData(7, 5, 5);
  pnl.pushData(8, 50, 50);
  pnl.pushData(9, 50, 25);
  pnl.pushData(10, 50, 30);

  pnl.calcMaxDrawdown();

  EXPECT_EQ(pnl.m_maxDrawdown, 28);
  EXPECT_EQ(pnl.m_maxDrawdownStartI, 2);
  EXPECT_EQ(pnl.m_maxDrawdownEndI, 5);
}

TEST(PNL, pnl_maxdrawdown3) {
  tr2::PNL pnl;

  pnl.pushData(1, 1, 1);
  pnl.pushData(2, 2, 2);
  pnl.pushData(3, 30, 30);
  pnl.pushData(4, 28, 28);
  pnl.pushData(5, 30, 30);
  pnl.pushData(6, 2, 2);
  pnl.pushData(7, 5, 5);
  pnl.pushData(8, 50, 50);
  pnl.pushData(9, 50, 22);
  pnl.pushData(10, 50, 30);

  pnl.calcMaxDrawdown();

  EXPECT_EQ(pnl.m_maxDrawdown, 28);
  EXPECT_EQ(pnl.m_maxDrawdownStartI, 7);
  EXPECT_EQ(pnl.m_maxDrawdownEndI, 8);
}