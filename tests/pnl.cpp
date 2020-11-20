#include <gtest/gtest.h>
#include <math.h>
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
  auto cnfund = tr2::ExchangeMgr::getSingleton()->newExchange("cnfund");

  for (auto i = 0; i < cnfund->getDataLength("110022"); i++) {
    tr2::CandleData cd;

    if (cnfund->getData("110022", i, cd)) {
      pnl.pushData(cd.ts, cd.close, cd.close);
    }
  }

  pnl.calcMaxDate();

  EXPECT_EQ(tr2::getDate(pnl.m_maxUpDay), 20150709);
  EXPECT_EQ(tr2::getDate(pnl.m_maxDownDay), 20200203);
  EXPECT_EQ(tr2::getDate(pnl.m_maxUpWeek), 20150518);
  EXPECT_EQ(tr2::getDate(pnl.m_maxDownWeek), 20150817);
  EXPECT_EQ(tr2::getDate(pnl.m_maxUpMonth), 20190102);
  EXPECT_EQ(tr2::getDate(pnl.m_maxDownMonth), 20181008);
  EXPECT_EQ(tr2::getDate(pnl.m_maxUpYear), 20190102);
  EXPECT_EQ(tr2::getDate(pnl.m_maxDownYear), 20180102);

  EXPECT_EQ(fabs(pnl.m_maxMoneyUpDay - 0.0814249) < 0.00001, true);
  EXPECT_EQ(fabs(pnl.m_maxMoneyDownDay - -0.0801617) < 0.00001, true);
  EXPECT_EQ(fabs(pnl.m_maxMoneyUpWeek - 0.10388693) < 0.00001, true);
  EXPECT_EQ(fabs(pnl.m_maxMoneyDownWeek - -0.1190301) < 0.00001, true);
  EXPECT_EQ(fabs(pnl.m_maxMoneyUpMonth - 0.1207483) < 0.00001, true);
  EXPECT_EQ(fabs(pnl.m_maxMoneyDownMonth - -0.1317707) < 0.00001, true);
  EXPECT_EQ(fabs(pnl.m_maxMoneyUpYear - 0.7329932) < 0.00001, true);
  EXPECT_EQ(fabs(pnl.m_maxMoneyDownYear - -0.2382579) < 0.00001, true);
}

TEST(PNL, pnl_variance) {
  tr2::PNL pnl;
  auto cnfund = tr2::ExchangeMgr::getSingleton()->newExchange("cnfund");

  for (auto i = 0; i < cnfund->getDataLength("110022"); i++) {
    tr2::CandleData cd;

    if (cnfund->getData("110022", i, cd)) {
      pnl.pushData(cd.ts, cd.close, cd.close);
    }
  }

  pnl.calcVariance();

  EXPECT_EQ(fabs(pnl.m_variance - 0.42337) < 0.00001, true);
}

TEST(PNL, pnl_varianc2) {
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

  pnl.calcVariance();

  EXPECT_DOUBLE_EQ(pnl.m_variance, 278);
}