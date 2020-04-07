#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

void startServ(const char* addr) {
  auto pServ = tr2::newTrainServer2(addr);
  pServ->run();
}

int main() {
  tr2::regAllIndicators();
  tr2::regAllExchanges();

  tr2::Config cfg;
  tr2::loadConfig(cfg, "../cfg/config.yaml.default");

  auto mgr = tr2::ExchangeMgr::getSingleton();
  mgr->init(cfg);

  startServ(cfg.bindaddr.c_str());

  return 0;
}