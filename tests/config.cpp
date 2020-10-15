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
  EXPECT_EQ(cfg.isLimitTasks, true);
  EXPECT_EQ(cfg.taskTimeOff, 100);
  EXPECT_EQ(cfg.tokens.size(), 1);
  EXPECT_EQ(cfg.tokens[0], "wzDkh9h2fhfUVuS9jZ8uVbhV3vC5AWX3");

  EXPECT_EQ(cfg.servs.size(), 1);
  EXPECT_EQ(cfg.servs[0].host, "0.0.0.0:3777");
  EXPECT_EQ(cfg.servs[0].token, "wzDkh9h2fhfUVuS9jZ8uVbhV3vC5AWX3");

  EXPECT_EQ(cfg.trdb2Serv, "127.0.0.1:5002");
  EXPECT_EQ(cfg.trdb2Token, "1234567");
}

TEST(config, loadConfig2) {
  tr2::Config cfg;
  auto ret = loadConfig(cfg, "../tests/test2.config.yaml");

  EXPECT_EQ(ret, true);

  EXPECT_EQ(cfg.cnfundPath, "../data/cnfund");

  EXPECT_EQ(cfg.exchanges.size(), 1);
  EXPECT_EQ(cfg.exchanges[0], "cnfund");

  EXPECT_EQ(cfg.bindaddr, "0.0.0.0:3777");
  EXPECT_EQ(cfg.taskNums, 0);
  EXPECT_EQ(cfg.isLimitTasks, false);
  EXPECT_EQ(cfg.taskTimeOff, 0);
  EXPECT_EQ(cfg.tokens.size(), 1);
  EXPECT_EQ(cfg.tokens[0], "wzDkh9h2fhfUVuS9jZ8uVbhV3vC5AWX3");

  EXPECT_EQ(cfg.servs.size(), 1);
  EXPECT_EQ(cfg.servs[0].host, "0.0.0.0:3777");
  EXPECT_EQ(cfg.servs[0].token, "wzDkh9h2fhfUVuS9jZ8uVbhV3vC5AWX3");

  EXPECT_EQ(cfg.trdb2Serv, "");
  EXPECT_EQ(cfg.trdb2Token, "");
}

TEST(config, loadConfig3) {
  tr2::Config cfg;
  auto ret = loadConfig(cfg, "../tests/test3.config.yaml");

  EXPECT_EQ(ret, true);

  EXPECT_EQ(cfg.cnfundPath, "../data/cnfund");

  EXPECT_EQ(cfg.exchanges.size(), 1);
  EXPECT_EQ(cfg.exchanges[0], "cnfund");

  EXPECT_EQ(cfg.bindaddr, "0.0.0.0:3777");
  EXPECT_EQ(cfg.taskNums, 0);
  EXPECT_EQ(cfg.isLimitTasks, false);
  EXPECT_EQ(cfg.taskTimeOff, 0);
  EXPECT_EQ(cfg.tokens.size(), 1);
  EXPECT_EQ(cfg.tokens[0], "wzDkh9h2fhfUVuS9jZ8uVbhV3vC5AWX3");

  EXPECT_EQ(cfg.servs.size(), 1);
  EXPECT_EQ(cfg.servs[0].host, "0.0.0.0:3777");
  EXPECT_EQ(cfg.servs[0].token, "wzDkh9h2fhfUVuS9jZ8uVbhV3vC5AWX3");
}