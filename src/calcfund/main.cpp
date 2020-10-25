#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

struct trData {
  int nums;
  float perValidData;
  float durationYear;
  time_t maxUpDay;
  time_t maxDownDay;
  time_t maxUpWeek;
  time_t maxDownWeek;
  time_t maxUpMonth;
  time_t maxDownMonth;
  time_t maxUpYear;
  time_t maxDownYear;
  float maxMoneyUpDay;
  float maxMoneyDownDay;
  float maxMoneyUpWeek;
  float maxMoneyDownWeek;
  float maxMoneyUpMonth;
  float maxMoneyDownMonth;
  float maxMoneyUpYear;
  float maxMoneyDownYear;
  time_t startMaxDrawdown;
  time_t endMaxDrawdown;
  std::string tags;
  time_t tsCreate;
  tr2::TrainResult tr;
};

struct mgrData {
  std::string name;
  time_t stOff;
  time_t etOff;

  mgrData(std::string _name, time_t _stOff, time_t _etOff)
      : name(_name), stOff(_stOff), etOff(_etOff) {}
};

void calcAllFunds(std::string fn, std::string strStart, std::string strEnd,
                  tr2::Config& cfg) {
  auto mgr = tr2::ExchangeMgr::getSingleton();
  mgr->init(cfg);
  auto cnfund = mgr->getExchange("cnfund");

  std::vector<trData> lst;

  int totalnums = 0;
  auto ret = tr2::getSymbols(
      cfg.trdb2Serv.c_str(), cfg.trdb2Token.c_str(), "cnfunds", NULL,
      [&](tradingdb2pb::SymbolInfo& si) {
        printf("onSymbol %s\n", si.fund().code().c_str());

        time_t st = 0;
        time_t et = 0;

        if (strStart.length() > 0) {
          st = tr2::str2timestampUTC(strStart.c_str(), "%Y%m%d");
        }

        if (strEnd.length() > 0) {
          et = tr2::str2timestampUTC(strEnd.c_str(), "%Y%m%d");
        }

        tradingdb2pb::Candles candles;
        auto ret = tr2::getCandles(candles, cfg.trdb2Serv.c_str(),
                                   cfg.trdb2Token.c_str(), "cnfunds",
                                   si.fund().code().c_str(), NULL, st, et - 1);

        printf("getCandles %s\n", ret ? "ok" : "fail");
        printf("candles %d\n", candles.candles_size());

        if (candles.candles_size() > 0) {
          tr2::PNL pnl;
          tr2::analysisFund(pnl, *cnfund, si, candles);

          // pnl.calcValidDataPer(si, *cnfund);

          trData trd;
          trd.nums = candles.candles_size();
          trd.perValidData = pnl.m_perValidData;
          trd.durationYear = pnl.m_durationYear;
          trd.maxUpDay = pnl.m_maxUpDay;
          trd.maxDownDay = pnl.m_maxDownDay;
          trd.maxUpWeek = pnl.m_maxUpWeek;
          trd.maxDownWeek = pnl.m_maxDownWeek;
          trd.maxUpMonth = pnl.m_maxUpMonth;
          trd.maxDownMonth = pnl.m_maxDownMonth;
          trd.maxUpYear = pnl.m_maxUpYear;
          trd.maxDownYear = pnl.m_maxDownYear;
          trd.maxMoneyUpDay = pnl.m_maxMoneyUpDay;
          trd.maxMoneyDownDay = pnl.m_maxMoneyDownDay;
          trd.maxMoneyUpWeek = pnl.m_maxMoneyUpWeek;
          trd.maxMoneyDownWeek = pnl.m_maxMoneyDownWeek;
          trd.maxMoneyUpMonth = pnl.m_maxMoneyUpMonth;
          trd.maxMoneyDownMonth = pnl.m_maxMoneyDownMonth;
          trd.maxMoneyUpYear = pnl.m_maxMoneyUpYear;
          trd.maxMoneyDownYear = pnl.m_maxMoneyDownYear;
          trd.tsCreate = si.fund().createtime();
          pnl.getTrainResult(trd.tr);
          trd.tr.name = si.fund().code().c_str();
          trd.startMaxDrawdown = pnl.getMaxDrawdownStartTime();
          trd.endMaxDrawdown = pnl.getMaxDrawdownEndTime();

          for (int i = 0; i < si.fund().tags_size(); ++i) {
            if (i > 0) {
              trd.tags.append(";");
            }
            trd.tags.append(si.fund().tags(i));
          }

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
            "annualizedVolatility,variance,perValidData,durationYear,maxupday,"
            "permaxupday,maxdownday,permaxdownday,maxupweek,permaxupweek,"
            "maxdownweek,permaxdownweek,maxupmonth,permaxupmonth,maxdownmonth,"
            "permaxdownmonth,maxupyear,permaxupyear,maxdownyear,"
            "permaxdownyear,tags,createtime,mddStart,mddEnd\r\n");
  };

  auto onrow = [&lst](FILE* fp, int row) {
    if (lst.size() <= row) {
      return false;
    }

    fprintf(fp,
            "%s,%d,%f,%f,%f,%f,%f,%f,%f,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,"
            "%d,%f,%d,%f,%s,%d,%d,%d\r\n",
            lst[row].tr.name.c_str(), lst[row].nums, lst[row].tr.totalReturn,
            lst[row].tr.maxDrawDown, lst[row].tr.sharpe,
            lst[row].tr.annualizedReturns, lst[row].tr.annualizedVolatility,
            lst[row].tr.variance, lst[row].perValidData, lst[row].durationYear,
            tr2::getDate(lst[row].maxUpDay), lst[row].maxMoneyUpDay,
            tr2::getDate(lst[row].maxDownDay), lst[row].maxMoneyDownDay,
            tr2::getDate(lst[row].maxUpWeek), lst[row].maxMoneyUpWeek,
            tr2::getDate(lst[row].maxDownWeek), lst[row].maxMoneyDownWeek,
            tr2::getDate(lst[row].maxUpMonth), lst[row].maxMoneyUpMonth,
            tr2::getDate(lst[row].maxDownMonth), lst[row].maxMoneyDownMonth,
            tr2::getDate(lst[row].maxUpYear), lst[row].maxMoneyUpYear,
            tr2::getDate(lst[row].maxDownYear), lst[row].maxMoneyDownYear,
            lst[row].tags.c_str(), tr2::getDate(lst[row].tsCreate),
            tr2::getDate(lst[row].startMaxDrawdown),
            tr2::getDate(lst[row].endMaxDrawdown));
    return true;
  };

  tr2::saveCSV(fn.c_str(), onhead, onrow);
}

void calcAllManagers(std::string fn, tr2::Config& cfg) {
  std::vector<mgrData> lstMgrData;
  lstMgrData.push_back(mgrData("full", 0, 0));
  lstMgrData.push_back(
      mgrData("off_1m", 30 * 24 * 60 * 60, -30 * 24 * 60 * 60));
  lstMgrData.push_back(
      mgrData("off_3m", 3 * 30 * 24 * 60 * 60, -3 * 30 * 24 * 60 * 60));

  auto mgr = tr2::ExchangeMgr::getSingleton();
  mgr->init(cfg);
  auto cnfund = mgr->getExchange("cnfund");

  std::vector<trData> lst;

  int totalnums = 0;
  auto ret = tr2::getSymbols(
      cfg.trdb2Serv.c_str(), cfg.trdb2Token.c_str(), "cnfunds", NULL,
      [&](tradingdb2pb::SymbolInfo& si) {
        printf("onSymbol %s\n", si.fund().code().c_str());
        tradingdb2pb::SymbolInfo nsi(si);
        auto matfund = nsi.mutable_fund();

        for (int i = 0; i < si.fund().managers_size(); ++i) {
          auto cm = si.fund().managers(i);
          auto matcm = matfund->mutable_managers(i);
          matcm->clear_results();

          for (auto it = lstMgrData.begin(); it != lstMgrData.end(); ++it) {
            time_t st = 0;
            time_t et = 0;

            if (cm.starttime() > 0) {
              st = cm.starttime() + it->stOff;
            }

            if (cm.endtime() > 0) {
              st = cm.endtime() + it->etOff + 24 * 60 * 60 - 1;
            }

            tradingdb2pb::Candles candles;
            auto ret = tr2::getCandles(candles, cfg.trdb2Serv.c_str(),
                                       cfg.trdb2Token.c_str(), "cnfunds",
                                       si.fund().code().c_str(), NULL, st, et);

            printf("getCandles %s\n", ret ? "ok" : "fail");
            printf("candles %d\n", candles.candles_size());

            if (candles.candles_size() > 0) {
              tr2::PNL pnl;
              tr2::analysisFund(pnl, *cnfund, si, candles);

              // pnl.calcValidDataPer(si, *cnfund);

              trData trd;
              trd.nums = candles.candles_size();
              trd.perValidData = pnl.m_perValidData;
              trd.durationYear = pnl.m_durationYear;
              trd.maxUpDay = pnl.m_maxUpDay;
              trd.maxDownDay = pnl.m_maxDownDay;
              trd.maxUpWeek = pnl.m_maxUpWeek;
              trd.maxDownWeek = pnl.m_maxDownWeek;
              trd.maxUpMonth = pnl.m_maxUpMonth;
              trd.maxDownMonth = pnl.m_maxDownMonth;
              trd.maxUpYear = pnl.m_maxUpYear;
              trd.maxDownYear = pnl.m_maxDownYear;
              trd.maxMoneyUpDay = pnl.m_maxMoneyUpDay;
              trd.maxMoneyDownDay = pnl.m_maxMoneyDownDay;
              trd.maxMoneyUpWeek = pnl.m_maxMoneyUpWeek;
              trd.maxMoneyDownWeek = pnl.m_maxMoneyDownWeek;
              trd.maxMoneyUpMonth = pnl.m_maxMoneyUpMonth;
              trd.maxMoneyDownMonth = pnl.m_maxMoneyDownMonth;
              trd.maxMoneyUpYear = pnl.m_maxMoneyUpYear;
              trd.maxMoneyDownYear = pnl.m_maxMoneyDownYear;
              trd.tsCreate = si.fund().createtime();
              pnl.getTrainResult(trd.tr);
              trd.tr.name = si.fund().code() + cm.name() + it->name;
              trd.startMaxDrawdown = pnl.getMaxDrawdownStartTime();
              trd.endMaxDrawdown = pnl.getMaxDrawdownEndTime();

              for (int i = 0; i < si.fund().tags_size(); ++i) {
                if (i > 0) {
                  trd.tags.append(";");
                }
                trd.tags.append(si.fund().tags(i));
              }

              printf(
                  "fund totalReturn: %f maxDrawDown: %f sharpe: %f "
                  "annualizedReturns: %f annualizedVolatility: %f variance: %f "
                  "perValidData: %f durationYear: %f\n",
                  trd.tr.totalReturn, trd.tr.maxDrawDown, trd.tr.sharpe,
                  trd.tr.annualizedReturns, trd.tr.annualizedVolatility,
                  trd.tr.variance, trd.perValidData, trd.durationYear);

              ++totalnums;

              lst.push_back(trd);

              auto cr = matcm->add_results();
              cr->set_name(it->name);
              cr->set_maxdrawdown(trd.tr.maxDrawDown);
              cr->set_annualizedreturns(trd.tr.annualizedReturns);
              cr->set_annualizedvolatility(trd.tr.annualizedVolatility);
              cr->set_totalreturns(trd.tr.totalReturn);
              cr->set_starttime(st);
              cr->set_endtime(et);
              cr->set_sharpe(trd.tr.sharpe);
            }
          }
        }

        tr2::updSymbol(cfg.trdb2Serv.c_str(), cfg.trdb2Token.c_str(), nsi);
      });

  printf("getSymbols %s\n", ret ? "ok" : "fail");
  printf("getSymbols %d\n", totalnums);

  auto onhead = [](FILE* fp) {
    fprintf(fp,
            "name,nums,totalReturn,maxDrawDown,sharpe,annualizedReturns,"
            "annualizedVolatility,variance,perValidData,durationYear,maxupday,"
            "permaxupday,maxdownday,permaxdownday,maxupweek,permaxupweek,"
            "maxdownweek,permaxdownweek,maxupmonth,permaxupmonth,maxdownmonth,"
            "permaxdownmonth,maxupyear,permaxupyear,maxdownyear,"
            "permaxdownyear,tags,createtime,mddStart,mddEnd\r\n");
  };

  auto onrow = [&lst](FILE* fp, int row) {
    if (lst.size() <= row) {
      return false;
    }

    fprintf(fp,
            "%s,%d,%f,%f,%f,%f,%f,%f,%f,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,"
            "%d,%f,%d,%f,%s,%d,%d,%d\r\n",
            lst[row].tr.name.c_str(), lst[row].nums, lst[row].tr.totalReturn,
            lst[row].tr.maxDrawDown, lst[row].tr.sharpe,
            lst[row].tr.annualizedReturns, lst[row].tr.annualizedVolatility,
            lst[row].tr.variance, lst[row].perValidData, lst[row].durationYear,
            tr2::getDate(lst[row].maxUpDay), lst[row].maxMoneyUpDay,
            tr2::getDate(lst[row].maxDownDay), lst[row].maxMoneyDownDay,
            tr2::getDate(lst[row].maxUpWeek), lst[row].maxMoneyUpWeek,
            tr2::getDate(lst[row].maxDownWeek), lst[row].maxMoneyDownWeek,
            tr2::getDate(lst[row].maxUpMonth), lst[row].maxMoneyUpMonth,
            tr2::getDate(lst[row].maxDownMonth), lst[row].maxMoneyDownMonth,
            tr2::getDate(lst[row].maxUpYear), lst[row].maxMoneyUpYear,
            tr2::getDate(lst[row].maxDownYear), lst[row].maxMoneyDownYear,
            lst[row].tags.c_str(), tr2::getDate(lst[row].tsCreate),
            tr2::getDate(lst[row].startMaxDrawdown),
            tr2::getDate(lst[row].endMaxDrawdown));
    return true;
  };

  tr2::saveCSV(fn.c_str(), onhead, onrow);
}

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

  // calcAllFunds("../output/calcfund.csv", "", "", cfg);
  calcAllManagers("../output/calcfundmanagers.csv", cfg);

  return 0;
}