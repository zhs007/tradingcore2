#include <tradingcore2/csv.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/indicatormgr.h>
#include <tradingcore2/strategy/singleindicator.h>
#include <tradingcore2/train.h>
#include <tradingcore2/utils.h>
#include <tradingcore2/wallet.h>

CR2BEGIN

void saveTrainResult(const char* fn, TrainResultList& lst) {
  int tradingNums;
  int stoplossNums;

  auto onhead = [](FILE* fp) {
    fprintf(fp,
            "name,totalReturn,maxDrawDown,sharpe,annualizedReturns,"
            "annualizedVolatility,tradingNums,stoplossNums,winRate\r\n");
  };

  auto onrow = [&lst](FILE* fp, int row) {
    if (lst.size() <= row) {
      return false;
    }

    fprintf(fp, "%s,%.2f%%,%.2f%%,%.3f,%.2f%%,%.2f%%,%d,%d,%.2f%%\r\n",
            lst[row].name.c_str(), lst[row].totalReturn * 100,
            lst[row].maxDrawDown * 100, lst[row].sharpe,
            lst[row].annualizedReturns * 100,
            lst[row].annualizedVolatility * 100, lst[row].tradingNums,
            lst[row].stoplossNums, lst[row].winRate * 100);

    return true;
  };

  tr2::saveCSV(fn, onhead, onrow);
}

bool calcIndicatorRange(Exchange& exchange, const char* assetsName,
                        const char* indicatorName, int avgtimes,
                        IndicatorDataValue& minval,
                        IndicatorDataValue& maxval) {
  auto mgr = IndicatorMgr::getSingleton();
  auto pIndicator = mgr->newIndicator(indicatorName, avgtimes);

  pIndicator->build(exchange, assetsName, 0,
                    exchange.getDataLength(assetsName));

  int index;
  auto pMin = pIndicator->getMinSingleValue(index);
  auto pMax = pIndicator->getMaxSingleValue(index);

  if (pMin != NULL && pMax != NULL) {
    minval = pMin->value;
    maxval = pMax->value;

    delete pIndicator;

    return true;
  }

  delete pIndicator;

  return false;
}

bool trainSingleIndicator(Exchange& exchange, const char* assetsName,
                          const char* indicatorName, const char* outputPath,
                          Money invest, int avgtimes, IndicatorDataValue off0,
                          IndicatorDataValue off1) {
  IndicatorDataValue minval, maxval;
  if (!calcIndicatorRange(exchange, assetsName, indicatorName, avgtimes, minval,
                          maxval)) {
    return false;
  }

  TrainResultList lst;

  for (auto cv0 = minval; cv0 <= maxval; cv0 += off0) {
    for (auto cv1 = cv0 + off1; cv1 <= maxval; cv1 += off0) {
      TrainResult tr;

      auto pWallet = new Wallet(exchange);
      pWallet->deposit(invest, exchange.getFirstTimeStamp());

      tr2::StrategySI* si = new tr2::StrategySI(*pWallet, exchange);
      si->init(assetsName, indicatorName, avgtimes, cv0, cv0 + off0, cv1,
               cv1 + off0, invest);
      si->setStopLess(0.10);

      si->simulateTrading();

      tr2::PNL pnl;
      pWallet->buildPNL(pnl);

      char strname[1024];
      sprintf(strname, "%s.%s.%.3f.%.3f", assetsName, indicatorName, cv0, cv1);

      pnl.print(strname);
      si->print();

      std::string fn;
      joinPath(fn, outputPath, strname);
      fn += ".csv";

      pnl.saveCSV(fn.c_str(), true);

      tr.name = strname;
      pnl.getTrainResult(tr);
      si->getTrainResult(tr);

      lst.push_back(tr);

      delete si;
      delete pWallet;
    }
  }

  std::string fn;
  joinPath(fn, outputPath, "train.csv");

  saveTrainResult(fn.c_str(), lst);

  return true;
}

CR2END