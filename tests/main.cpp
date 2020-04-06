#include <gtest/gtest.h>
#include <tradingcore2/tradingcore2.h>

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  tr2::regAllIndicators();
  tr2::regAllExchanges();

  tr2::Config cfg;
  tr2::loadConfig(cfg, "../cfg/config.yaml.default");

  auto mgr = tr2::ExchangeMgr::getSingleton();
  mgr->init(cfg);

  return RUN_ALL_TESTS();
}