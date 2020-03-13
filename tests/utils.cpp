#include <gtest/gtest.h>
#include <stdio.h>
#include <tradingcore2/utils.h>

TEST(Utils, str2time) {
  tm t;

  tr2::str2time("20170117", "%Y%m%d", t);
  EXPECT_EQ(t.tm_year, 2017 - 1900);
  EXPECT_EQ(t.tm_mon, 0);
  EXPECT_EQ(t.tm_mday, 17);

  tr2::str2time("20191231", "%Y%m%d", t);
  EXPECT_EQ(t.tm_year, 2019 - 1900);
  EXPECT_EQ(t.tm_mon, 11);
  EXPECT_EQ(t.tm_mday, 31);

  tr2::str2time("20200230", "%Y%m%d", t);
  EXPECT_EQ(t.tm_year, 2020 - 1900);
  EXPECT_EQ(t.tm_mon, 1);
  EXPECT_EQ(t.tm_mday, 30);
}

TEST(Utils, str2timestampUTC) {
  auto ts = tr2::str2timestampUTC("20170117", "%Y%m%d");
  EXPECT_EQ(ts, 1484611200);
}

TEST(Utils, time2timestampUTC) {
  tm t;
  memset(&t, 0, sizeof(tm));

  t.tm_year = 2019 - 1900;
  t.tm_mon = 0;
  t.tm_mday = 1;

  auto ts = tr2::time2timestampUTC(t);
  EXPECT_EQ(ts, 1546300800);
}