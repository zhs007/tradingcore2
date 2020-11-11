#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

int main(int argc, char *argv[]) {
  tr2::LogHelper log(argv[0]);

  printf("trdb2client starting...\n");
  printf("version is %s\n", tr2::getVersion());

  if (argc != 2) {
    printf("please type trdb2client cfgfile.");

    return -1;
  }

  tr2::Config cfg;
  tr2::loadConfig(cfg, argv[1]);

  tradingpb::Candles candles;
  auto ret =
      tr2::getCandles(candles, cfg.trdb2Serv.c_str(), cfg.trdb2Token.c_str(),
                      "cnfunds", "000001", NULL, 0, 0);

  printf("getCandles %s\n", ret ? "ok" : "fail");
  printf("candles %d\n", candles.candles_size());

  return 0;
}