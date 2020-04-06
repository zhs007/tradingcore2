#include <gtest/gtest.h>
#include <stdio.h>
#include <tradingcore2/csv.h>

TEST(CSV, csvloader) {
  tr2::CSVLoader csv;

  csv.load("../data/cnfund/110022.csv");

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

TEST(CSV, savecsv) {
  struct testobj {
    int a;
    float b;
    std::string c;

    testobj(int _a, float _b, const char* _c) : a(_a), b(_b), c(_c) {}
  };

  std::vector<testobj> lst;

  lst.push_back(testobj(1, 0.2, "3s"));
  lst.push_back(testobj(11, 2.2, "3s3s"));

  auto onhead = [](FILE* fp) { fprintf(fp, "column0,column1,column2\r\n"); };

  auto onrow = [&lst](FILE* fp, int row) {
    if (lst.size() <= row) {
      return false;
    }

    fprintf(fp, "%d,%.2f,%s\r\n", lst[row].a, lst[row].b, lst[row].c.c_str());
    return true;
  };

  tr2::saveCSV("../data/test.savecsv.csv", onhead, onrow);

  tr2::CSVLoader csv;

  csv.load("../data/test.savecsv.csv");

  EXPECT_EQ(csv.getWidth(), 3);
  EXPECT_EQ(csv.getHeight(), 2);

  EXPECT_STREQ(csv.getHead(0), "column0");
  EXPECT_STREQ(csv.getHead(1), "column1");
  EXPECT_STREQ(csv.getHead(2), "column2");

  EXPECT_STREQ(csv.getData(0, 0), "1");
  EXPECT_STREQ(csv.getData(1, 0), "0.20");
  EXPECT_STREQ(csv.getData(2, 0), "3s");

  EXPECT_STREQ(csv.getData(0, 1), "11");
  EXPECT_STREQ(csv.getData(1, 1), "2.20");
  EXPECT_STREQ(csv.getData(2, 1), "3s3s");
}