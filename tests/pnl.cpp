#include <gtest/gtest.h>
#include <stdio.h>
#include <tradingcore2/exchangemgr.h>
#include <tradingcore2/pnl.h>
#include <tradingcore2/utils.h>

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

  EXPECT_DOUBLE_EQ(pnl.m_maxDrawdown, 28.0f / 30.0f);
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

  EXPECT_DOUBLE_EQ(pnl.m_maxDrawdown, 28.0f / 30.0f);
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

  EXPECT_DOUBLE_EQ(pnl.m_maxDrawdown, 28.0f / 30.0f);
  EXPECT_EQ(pnl.m_maxDrawdownStartI, 2);
  EXPECT_EQ(pnl.m_maxDrawdownEndI, 5);
}

TEST(PNL, pnl_maxupdown) {
  tr2::PNL pnl;
  auto cnfund = tr2::ExchangeMgr::getSingleton()->getExchange("cnfund");

  for (auto i = 0; i < cnfund->getDataLength("110022"); i++) {
    tr2::CandleData cd;

    if (cnfund->getData("110022", i, cd)) {
      pnl.pushData(cd.ts, cd.close, cd.close);
    }
  }

  pnl.calcMaxDate();

  // EXPECT_EQ(tr2::getYear(pnl.m_maxUpDay), 2019);
  // EXPECT_EQ(pnl.m_maxDownDay, 5);
  // EXPECT_EQ(pnl.m_maxUpWeek, 5);
  // EXPECT_EQ(pnl.m_maxDownWeek, 5);
  // EXPECT_EQ(pnl.m_maxUpMonth, 5);
  // EXPECT_EQ(pnl.m_maxDownMonth, 5);
  // EXPECT_EQ(pnl.m_maxUpYear, 5);
  // EXPECT_EQ(pnl.m_maxDownYear, 5);

  // EXPECT_EQ(pnl.m_maxMoneyUpDay, 5);
  // EXPECT_EQ(pnl.m_maxMoneyDownDay, 5);
  // EXPECT_EQ(pnl.m_maxMoneyUpWeek, 5);
  // EXPECT_EQ(pnl.m_maxMoneyDownWeek, 5);
  // EXPECT_EQ(pnl.m_maxMoneyUpMonth, 5);
  // EXPECT_EQ(pnl.m_maxMoneyDownMonth, 5);
  // EXPECT_EQ(pnl.m_maxMoneyUpYear, 5);
  // EXPECT_EQ(pnl.m_maxMoneyDownYear, 5);
}