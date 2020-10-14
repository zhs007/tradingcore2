#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

struct trData {
  int nums;
  float perValidData;
  float durationYear;
  tr2::TrainResult tr;
};

int main(int argc, char* argv[]) {
  tr2::LogHelper log(argv[0]);

  printf("trdb2client starting...\n");
  printf("version is %s\n", tr2::getVersion());

  if (argc != 2) {
    printf("please type trdb2client cfgfile.");

    return -1;
  }

  tr2::regAllIndicators();
  tr2::regAllExchanges();

  tr2::Config cfg;
  tr2::loadConfig(cfg, argv[1]);

  auto mgr = tr2::ExchangeMgr::getSingleton();
  mgr->init(cfg);
  auto cnfund = mgr->getExchange("cnfund");

  std::vector<trData> lst;

  int totalnums = 0;
  auto ret = tr2::getSymbols(
      cfg.trdb2Serv.c_str(), cfg.trdb2Token.c_str(), "cnfunds", NULL,
      [&](tradingdb2pb::SymbolInfo& si) {
        printf("onSymbol %s\n", si.fund().code().c_str());

        tradingdb2pb::Candles candles;
        auto ret = tr2::getCandles(candles, cfg.trdb2Serv.c_str(),
                                   cfg.trdb2Token.c_str(), "cnfunds",
                                   si.fund().code().c_str(), NULL, 0, 0);

        printf("getCandles %s\n", ret ? "ok" : "fail");
        printf("candles %d\n", candles.candles_size());

        if (candles.candles_size() > 0) {
          tr2::PNL pnl;
          tr2::analysisFund(pnl, *cnfund, candles);

          pnl.calcValidDataPer(si, *cnfund);

          trData trd;
          trd.nums = candles.candles_size();
          trd.perValidData = pnl.m_perValidData;
          trd.durationYear = pnl.m_durationYear;
          pnl.getTrainResult(trd.tr);
          trd.tr.name = si.fund().code().c_str();

          printf(
              "fund totalReturn: %f maxDrawDown: %f sharpe: %f "
              "annualizedReturns: %f annualizedVolatility: %f variance: %f "
              "perValidData: %f durationYear: %f\n",
              trd.tr.totalReturn, trd.tr.maxDrawDown, trd.tr.sharpe,
              trd.tr.annualizedReturns, trd.tr.annualizedVolatility,
              trd.tr.variance, trd.perValidData, trd.durationYear);

          ++totalnums;

          lst.push_back(trd);
        }
      });

  printf("getSymbols %s\n", ret ? "ok" : "fail");
  printf("getSymbols %d\n", totalnums);

  auto onhead = [](FILE* fp) {
    fprintf(fp,
            "code,nums,totalReturn,maxDrawDown,sharpe,annualizedReturns,"
            "annualizedVolatility,variance,perValidData,durationYear\r\n");
  };

  auto onrow = [&lst](FILE* fp, int row) {
    if (lst.size() <= row) {
      return false;
    }

    fprintf(fp, "%s,%d,%f,%f,%f,%f,%f,%f,%f,%f\r\n", lst[row].tr.name.c_str(),
            lst[row].nums, lst[row].tr.totalReturn, lst[row].tr.maxDrawDown,
            lst[row].tr.sharpe, lst[row].tr.annualizedReturns,
            lst[row].tr.annualizedVolatility, lst[row].tr.variance,
            lst[row].perValidData, lst[row].durationYear);
    return true;
  };

  tr2::saveCSV("../output/calcfund.csv", onhead, onrow);

  return 0;
}