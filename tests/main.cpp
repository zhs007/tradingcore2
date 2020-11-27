#include <gtest/gtest.h>
#include <tradingcore2/tradingcore2.h>

int main(int argc, char *argv[]) {
  putenv("TZ=UTC");

  tr2::LogHelper log(argv[0]);

  ::testing::InitGoogleTest(&argc, argv);

  LOG(INFO) << "timezone " << timezone;

  tr2::regAllIndicators();
  tr2::regAllExchanges();
  tr2::regAllCtrlConditionHelper();
  tr2::regAllStrategy();

  tr2::Config cfg;
  tr2::loadConfig(cfg, "../cfg/config.yaml.default");

  auto mgr = tr2::ExchangeMgr::getSingleton();
  mgr->init(cfg);

  return RUN_ALL_TESTS();
}