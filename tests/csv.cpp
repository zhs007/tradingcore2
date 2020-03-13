#include <gtest/gtest.h>
#include <stdio.h>
#include <tradingcore2/csv.h>

TEST(CSV, csvloader) {
  tr2::CSVLoader csv;

  csv.load("../data/110022.csv");

  EXPECT_EQ(csv.getWidth(), 4);
  EXPECT_EQ(csv.getHeight(), 2280);

  EXPECT_STREQ(csv.getHead(0), "code");
  EXPECT_STREQ(csv.getHead(1), "date");
  EXPECT_STREQ(csv.getHead(2), "value");
  EXPECT_STREQ(csv.getHead(3), "totalvalue");

  EXPECT_STREQ(csv.getData(0, 0), "110022");
  EXPECT_STREQ(csv.getData(1, 0), "20100820");
  EXPECT_STREQ(csv.getData(2, 0), "10000");
  EXPECT_STREQ(csv.getData(3, 0), "10000");

  EXPECT_STREQ(csv.getData(0, 1), "110022");
  EXPECT_STREQ(csv.getData(1, 1), "20100827");
  EXPECT_STREQ(csv.getData(2, 1), "10009");
  EXPECT_STREQ(csv.getData(3, 1), "10009");

  EXPECT_STREQ(csv.getData(0, 2279), "110022");
  EXPECT_STREQ(csv.getData(1, 2279), "20200212");
  EXPECT_STREQ(csv.getData(2, 2279), "29150");
  EXPECT_STREQ(csv.getData(3, 2279), "29150");
}