#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

void train(const tr2::Config& cfg) {
  auto cnfund = tr2::ExchangeMgr::getSingleton()->getExchange("cnfund");
  tr2::startTrainSingleIndicator2ExPool(cfg, *cnfund, "110022", "rsi",
                                        "../output", 10000, 5, 5 /* off0 */, 5,
                                        5 /* off2 */, 10, 2);
}

int main(int argc, char* argv[]) {
  tr2::LogHelper log(argv[0]);

  printf("tr2client starting...\n");
  printf("version is %s\n", tr2::getVersion());

  if (argc != 2) {
    printf("please type client cfgfile.");

    return -1;
  }

  tr2::regAllIndicators();
  tr2::regAllExchanges();

  tr2::Config cfg;
  tr2::loadConfig(cfg, argv[1]);

  auto mgr = tr2::ExchangeMgr::getSingleton();
  mgr->init(cfg);

  train(cfg);

  return 0;
}