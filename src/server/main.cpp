#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

void startServ(const char* addr) {
  auto pServ = tr2::newTrainServer2(addr);
  pServ->run();
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("please type server cfgfile.");

    return -1;
  }

  tr2::regAllIndicators();
  tr2::regAllExchanges();

  tr2::Config cfg;
  tr2::loadConfig(cfg, argv[1]);

  auto mgr = tr2::ExchangeMgr::getSingleton();
  mgr->init(cfg);

  startServ(cfg.bindaddr.c_str());

  return 0;
}