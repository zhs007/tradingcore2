#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

void train(const char* addr) {
  auto cnfund = tr2::ExchangeMgr::getSingleton()->getExchange("cnfund");
  tr2::startTrainSingleIndicator2Ex(addr, *cnfund, "110022", "rsi", "../output",
                                    10000, 5, 5 /* off0 */, 5, 5 /* off2 */, 10,
                                    2);
}

int main() {
  tr2::regAllIndicators();
  tr2::regAllExchanges();

  tr2::Config cfg;
  tr2::loadConfig(cfg, "../cfg/config.yaml.default");

  auto mgr = tr2::ExchangeMgr::getSingleton();
  mgr->init(cfg);

  train(cfg.servs[0].c_str());

  return 0;
}