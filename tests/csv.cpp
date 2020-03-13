#include <gtest/gtest.h>
#include <stdio.h>
#include <tradingcore2/csv.h>

TEST(CSV, csvloader) {
  tr2::CSVLoader csv;

  csv.load("../data/110022.csv");

  EXPECT_EQ(csv.getWidth(), 4);
  EXPECT_EQ(csv.getHeight(), 2281);
}