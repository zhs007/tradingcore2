#include <gtest/gtest.h>
#include <tradingcore2/allindicators.h>

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  tr2::regAllIndicators();

  return RUN_ALL_TESTS();
}