#include <gtest/gtest.h>
#include <tradingcore2/config.h>

TEST(config, loadConfig) {
  tr2::Config cfg;
  auto ret = loadConfig(cfg, "../cfg/config.yaml.default");

  EXPECT_EQ(ret, true);
  EXPECT_EQ(cfg.cnfundpath, "./data/cnfund");
  EXPECT_EQ(cfg.exchanges.size(), 1);
  EXPECT_EQ(cfg.exchanges[0], "cnfund");
}