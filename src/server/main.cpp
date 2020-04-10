#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

void startServ(const tr2::Config& cfg) {
  auto pServ = tr2::newTrainServer2(cfg);
  //   pServ->init(cfg);
  pServ->run();
}

int main(int argc, char* argv[]) {
  printf("tr2serv starting...\nversion is %s\n", tr2::getVersion());

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

  startServ(cfg);

  return 0;
}