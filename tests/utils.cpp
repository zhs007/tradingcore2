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

TEST(Utils, calcQuartile0) {
  float arr[] = {6, 7, 15, 36, 39, 40, 41, 42, 43, 47, 49};

  float q1, q2, q3;
  tr2::calcQuartile(arr, 1, sizeof(arr) / sizeof(float), q1, q2, q3);

  EXPECT_EQ(q1, 15);
  EXPECT_EQ(q2, 40);
  EXPECT_EQ(q3, 43);
}

TEST(Utils, calcQuartile1) {
  float arr[] = {7, 15, 36, 39, 40, 41};

  float q1, q2, q3;
  tr2::calcQuartile(arr, 1, sizeof(arr) / sizeof(float), q1, q2, q3);

  EXPECT_EQ(q1, 13);
  EXPECT_EQ(q2, 37.5);
  EXPECT_EQ(q3, 40.25);
}

TEST(Utils, joinPath) {
  auto str = tr2::joinPath("dir0", "f1.txt");
  EXPECT_EQ(str, "dir0/f1.txt");

  str = tr2::joinPath("dir0/", "f1.txt");
  EXPECT_EQ(str, "dir0/f1.txt");
}

TEST(Utils, scaleValue) {
  auto r = tr2::scaleValue(1.23, 0.05);
  EXPECT_EQ(r, 1.25);

  r = tr2::scaleValue(-1.23, 0.05);
  EXPECT_EQ(r, -1.25);

  r = tr2::scaleValue(2.5, 0.05);
  EXPECT_EQ(r, 2.5);

  r = tr2::scaleValue(-12.5, 0.05);
  EXPECT_EQ(r, -12.5);

  r = tr2::scaleValue(0, 0.05);
  EXPECT_EQ(r, 0);
}

TEST(Utils, in_foreachPathWithExt) {
  const char* dir = "abc.csv";
  auto ci = strstr(dir, ".csv");

  EXPECT_EQ(ci - dir, strlen(dir) - strlen(".csv"));
}

TEST(Utils, foreachPathWithExt) {
  auto onfile = [](const char* dir, const char* fn) {
    EXPECT_TRUE(strcmp(fn, "110022.csv") == 0);
  };

  tr2::foreachPathWithExt("../data/cnfund", ".csv", onfile);
}