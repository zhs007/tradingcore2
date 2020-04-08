#include <gtest/gtest.h>
#include <tradingcore2/config.h>

TEST(config, loadConfig) {
  tr2::Config cfg;
  auto ret = loadConfig(cfg, "../cfg/config.yaml.default");

  EXPECT_EQ(ret, true);

  EXPECT_EQ(cfg.cnfundPath, "../data/cnfund");

  EXPECT_EQ(cfg.exchanges.size(), 1);
  EXPECT_EQ(cfg.exchanges[0], "cnfund");

  EXPECT_EQ(cfg.bindaddr, "0.0.0.0:3777");
  EXPECT_EQ(cfg.taskNums, -1);
  EXPECT_EQ(cfg.tokens.size(), 1);
  EXPECT_EQ(cfg.tokens[0], "wzDkh9h2fhfUVuS9jZ8uVbhV3vC5AWX3");

  EXPECT_EQ(cfg.servs.size(), 1);
  EXPECT_EQ(cfg.servs[0], "0.0.0.0:3777");
  EXPECT_EQ(cfg.token, "wzDkh9h2fhfUVuS9jZ8uVbhV3vC5AWX3");
}

TEST(config, loadConfig2) {
  tr2::Config cfg;
  auto ret = loadConfig(cfg, "../tests/test.config.yaml");

  EXPECT_EQ(ret, true);

  EXPECT_EQ(cfg.cnfundPath, "../data/cnfund");

  EXPECT_EQ(cfg.exchanges.size(), 1);
  EXPECT_EQ(cfg.exchanges[0], "cnfund");

  EXPECT_EQ(cfg.bindaddr, "0.0.0.0:3777");
  EXPECT_EQ(cfg.taskNums, 0);
  EXPECT_EQ(cfg.tokens.size(), 1);
  EXPECT_EQ(cfg.tokens[0], "wzDkh9h2fhfUVuS9jZ8uVbhV3vC5AWX3");

  EXPECT_EQ(cfg.servs.size(), 1);
  EXPECT_EQ(cfg.servs[0], "0.0.0.0:3777");
  EXPECT_EQ(cfg.token, "wzDkh9h2fhfUVuS9jZ8uVbhV3vC5AWX3");
}