#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

void startServ(const tr2::Config& cfg) {
  auto pServ = tr2::newNodeServer2(cfg);
  //   pServ->init(cfg);
  pServ->run();
}

int main(int argc, char* argv[]) {
  putenv("TZ=UTC");
  tr2::LogHelper log(argv[0]);

  LOG(INFO) << "tr2serv (" << tr2::getVersion() << ") starting...";
  // LOG(INFO) << "tr2serv starting...";
  // printf("tr2serv starting...\n");
  // printf("version is %s\n", tr2::getVersion());

  if (argc != 2) {
    LOG(ERROR) << "tr2serv argv file.";
    // printf("please type server cfgfile.\n");

    return -1;
  }

  tr2::regAllIndicators();
  tr2::regAllExchanges();

  tr2::Config cfg;
  tr2::loadConfig(cfg, argv[1]);

  auto mgr = tr2::ExchangeMgr::getSingleton();
  mgr->init(cfg);

  LOG(INFO) << "tr2serv started.";

  startServ(cfg);

  LOG(INFO) << "tr2serv exit.";

  return 0;
}