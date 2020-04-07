#include <tradingcore2/csv.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/indicatormgr.h>
#include <tradingcore2/strategy/singleindicator.h>
#include <tradingcore2/strategy/singleindicator2.h>
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

// 统计计算次数
int calcSingleIndicatorTimes(IndicatorDataValue minval,
                             IndicatorDataValue maxval, IndicatorDataValue off0,
                             IndicatorDataValue off1, IndicatorDataValue off2,
                             IndicatorDataValue maxoff2) {
  int times = 0;
  for (auto cv0 = minval; cv0 <= maxval; cv0 += off0) {
    for (auto cv0off = off2; cv0off <= maxoff2; cv0off += off2) {
      for (auto cv1 = cv0 + off1; cv1 <= maxval; cv1 += off0) {
        for (auto cv1off = off2; cv1off <= maxoff2; cv1off += off2) {
          times++;
        }
      }
    }
  }

  return times;
}

bool trainSingleIndicator(Exchange& exchange, const char* assetsName,
                          const char* indicatorName, const char* outputPath,
                          Money invest, int avgtimes, IndicatorDataValue off0,
                          IndicatorDataValue off1, IndicatorDataValue off2,
                          IndicatorDataValue maxoff2, float minValidReturn) {
  IndicatorDataValue minval, maxval;
  if (!calcIndicatorRange(exchange, assetsName, indicatorName, avgtimes, minval,
                          maxval)) {
    return false;
  }

  TrainResultList lst;

  minval = scaleValue(minval, off0);
  maxval = scaleValue(maxval, off0);
  auto maxtimes =
      calcSingleIndicatorTimes(minval, maxval, off0, off1, off2, maxoff2);
  int curtimes = 0;
  auto st = time(NULL);

  for (auto cv0 = minval; cv0 <= maxval; cv0 += off0) {
    for (auto cv0off = off2; cv0off <= maxoff2; cv0off += off2) {
      for (auto cv1 = cv0 + off1; cv1 <= maxval; cv1 += off0) {
        for (auto cv1off = off2; cv1off <= maxoff2; cv1off += off2) {
          curtimes++;
          TrainResult tr;

          auto pWallet = new Wallet(exchange);
          pWallet->deposit(invest, exchange.getFirstTimeStamp());

          tr2::StrategySI* si = new tr2::StrategySI(*pWallet, exchange);
          si->init(assetsName, indicatorName, avgtimes, cv0, cv0 + cv0off, cv1,
                   cv1 + cv1off, invest);
          si->setStopLess(0.10);

          si->simulateTrading();

          tr2::PNL pnl;
          pWallet->buildPNL(pnl);

          char strname[1024];
          sprintf(strname, "%s.%s-[%.3f:%.3f].[%.3f:%.3f]", assetsName,
                  indicatorName, cv0, cv0 + cv0off, cv1, cv1 + cv1off);

          pnl.print(strname);
          si->print();

          tr.name = strname;
          pnl.getTrainResult(tr);
          si->getTrainResult(tr);

          if (tr.totalReturn > minValidReturn) {
            auto fn1 = joinPath(outputPath, strname);
            fn1 += ".csv";

            pnl.saveCSV(fn1.c_str(), true);
          }

          lst.push_back(tr);

          delete si;
          delete pWallet;

          auto ct = time(NULL);
          printf("Progress is %.2f%% (%d/%d)\n", 100.0f * curtimes / maxtimes,
                 curtimes, maxtimes);

          if (ct > st) {
            printf("current time is %d s, last time is %d s\n", (int)(ct - st),
                   (int)((ct - st) * maxtimes / curtimes) - (int)(ct - st));
          }
        }
      }
    }
  }

  auto fn = joinPath(outputPath, "train.csv");

  saveTrainResult(fn.c_str(), lst);

  return true;
}

// 统计计算次数
int calcSingleIndicatorTimes2(IndicatorDataValue minval,
                              IndicatorDataValue maxval,
                              IndicatorDataValue off0, IndicatorDataValue off1,
                              IndicatorDataValue off2,
                              IndicatorDataValue maxoff2) {
  int times = 0;
  for (auto cv0 = minval; cv0 <= maxval; cv0 += off0) {
    for (auto cv0off = off2; cv0off <= maxoff2; cv0off += off2) {
      for (auto cv0dir = 0; cv0dir <= 1; cv0dir++) {
        for (auto cv1 = cv0 + off1; cv1 <= maxval; cv1 += off0) {
          for (auto cv1off = off2; cv1off <= maxoff2; cv1off += off2) {
            for (auto cv1dir = 0; cv1dir <= 1; cv1dir++) {
              times++;
            }
          }
        }
      }
    }
  }

  return times;
}

// trainSingleIndicator2 - 单独一个indicator的策略，考虑方向
bool trainSingleIndicator2(Exchange& exchange, const char* assetsName,
                           const char* indicatorName, const char* outputPath,
                           Money invest, int avgtimes, IndicatorDataValue off0,
                           IndicatorDataValue off1, IndicatorDataValue off2,
                           IndicatorDataValue maxoff2, float minValidReturn) {
  IndicatorDataValue minval, maxval;
  if (!calcIndicatorRange(exchange, assetsName, indicatorName, avgtimes, minval,
                          maxval)) {
    return false;
  }

  TrainResultList lst;

  minval = scaleValue(minval, off0);
  maxval = scaleValue(maxval, off0);
  auto maxtimes =
      calcSingleIndicatorTimes2(minval, maxval, off0, off1, off2, maxoff2);
  int curtimes = 0;
  auto st = time(NULL);

  for (auto cv0 = minval; cv0 <= maxval; cv0 += off0) {
    for (auto cv0off = off2; cv0off <= maxoff2; cv0off += off2) {
      for (auto cv0dir = 0; cv0dir <= 1; cv0dir++) {
        for (auto cv1 = cv0 + off1; cv1 <= maxval; cv1 += off0) {
          for (auto cv1off = off2; cv1off <= maxoff2; cv1off += off2) {
            for (auto cv1dir = 0; cv1dir <= 1; cv1dir++) {
              curtimes++;
              TrainResult tr;

              auto pWallet = new Wallet(exchange);
              pWallet->deposit(invest, exchange.getFirstTimeStamp());

              auto pStrategy = new tr2::StrategySI2(*pWallet, exchange);

              auto minbuy = cv0;
              auto maxbuy = cv0 + cv0off;
              auto onbuy = [&minbuy, &maxbuy, &cv0dir](
                               Exchange* pExchange, Indicator* pIndicator,
                               TimeStamp ts, int index) -> bool {
                if (index <= 0) {
                  return false;
                }

                auto lastcv = pIndicator->getSingleValue(index - 1);
                assert(lastcv);

                auto cv = pIndicator->getSingleValue(index);
                assert(cv);

                if (cv0dir == 0) {
                  if (cv->value < lastcv->value) {
                    return false;
                  }
                } else {
                  if (cv->value > lastcv->value) {
                    return false;
                  }
                }

                if (cv->value >= minbuy && cv->value < maxbuy) {
                  return true;
                }

                return false;
              };

              auto minsell = cv1;
              auto maxsell = cv1 + cv1off;
              auto onsell = [&minsell, &maxsell, &cv1dir](
                                Exchange* pExchange, Indicator* pIndicator,
                                TimeStamp ts, int index) -> bool {
                if (index <= 0) {
                  return false;
                }

                auto lastcv = pIndicator->getSingleValue(index - 1);
                assert(lastcv);

                auto cv = pIndicator->getSingleValue(index);
                assert(cv);

                if (cv1dir == 0) {
                  if (cv->value < lastcv->value) {
                    return false;
                  }
                } else {
                  if (cv->value > lastcv->value) {
                    return false;
                  }
                }

                if (cv->value >= minsell && cv->value < maxsell) {
                  return true;
                }

                return false;
              };

              pStrategy->init(assetsName, indicatorName, avgtimes, onbuy,
                              onsell, invest);
              pStrategy->setStopLess(0.10);

              pStrategy->simulateTrading();

              tr2::PNL pnl;
              pWallet->buildPNL(pnl);

              char strname[1024];
              sprintf(strname, "%s.%s-%d[%.3f:%.3f].%d[%.3f:%.3f]", assetsName,
                      indicatorName, cv0dir, minbuy, maxbuy, cv1dir, minsell,
                      maxsell);

              pnl.print(strname);
              pStrategy->print();

              tr.name = strname;
              pnl.getTrainResult(tr);
              pStrategy->getTrainResult(tr);

              if (tr.totalReturn > minValidReturn) {
                auto fn1 = joinPath(outputPath, strname);
                fn1 += ".csv";

                pnl.saveCSV(fn1.c_str(), true);
              }

              lst.push_back(tr);

              delete pStrategy;
              delete pWallet;

              auto ct = time(NULL);
              printf("Progress is %.2f%% (%d/%d)\n",
                     100.0f * curtimes / maxtimes, curtimes, maxtimes);

              if (ct > st) {
                printf("current time is %d s, last time is %d s\n",
                       (int)(ct - st),
                       (int)((ct - st) * maxtimes / curtimes) - (int)(ct - st));
              }
            }
          }
        }
      }
    }
  }

  auto fn = joinPath(outputPath, "train.csv");

  saveTrainResult(fn.c_str(), lst);

  return true;
}

// _trainSingleIndicator2Ex -
// 单独一个indicator的策略，考虑方向，一次只算若干次数，用于server分布式运算
bool _trainSingleIndicator2Ex(TrainResultList& lst, Exchange& exchange,
                              const char* assetsName, const char* indicatorName,
                              const char* outputPath, Money invest,
                              int avgtimes, IndicatorDataValue off0,
                              IndicatorDataValue off1, IndicatorDataValue off2,
                              IndicatorDataValue maxoff2, float minValidReturn,
                              IndicatorDataValue minval,
                              IndicatorDataValue maxval, IndicatorDataValue cv0,
                              IndicatorDataValue cv0off) {
  // for (auto cv0 = minv0; cv0 <= maxv0; cv0 += off0) {
  // for (auto cv0off = off2; cv0off <= maxoff2; cv0off += off2) {
  for (auto cv0dir = 0; cv0dir <= 1; cv0dir++) {
    for (auto cv1 = cv0 + off1; cv1 <= maxval; cv1 += off0) {
      for (auto cv1off = off2; cv1off <= maxoff2; cv1off += off2) {
        for (auto cv1dir = 0; cv1dir <= 1; cv1dir++) {
          TrainResult tr;

          auto pWallet = new Wallet(exchange);
          pWallet->deposit(invest, exchange.getFirstTimeStamp());

          auto pStrategy = new tr2::StrategySI2(*pWallet, exchange);

          auto minbuy = cv0;
          auto maxbuy = cv0 + cv0off;
          auto onbuy = [&minbuy, &maxbuy, &cv0dir](
                           Exchange* pExchange, Indicator* pIndicator,
                           TimeStamp ts, int index) -> bool {
            if (index <= 0) {
              return false;
            }

            auto lastcv = pIndicator->getSingleValue(index - 1);
            assert(lastcv);

            auto cv = pIndicator->getSingleValue(index);
            assert(cv);

            if (cv0dir == 0) {
              if (cv->value < lastcv->value) {
                return false;
              }
            } else {
              if (cv->value > lastcv->value) {
                return false;
              }
            }

            if (cv->value >= minbuy && cv->value < maxbuy) {
              return true;
            }

            return false;
          };

          auto minsell = cv1;
          auto maxsell = cv1 + cv1off;
          auto onsell = [&minsell, &maxsell, &cv1dir](
                            Exchange* pExchange, Indicator* pIndicator,
                            TimeStamp ts, int index) -> bool {
            if (index <= 0) {
              return false;
            }

            auto lastcv = pIndicator->getSingleValue(index - 1);
            assert(lastcv);

            auto cv = pIndicator->getSingleValue(index);
            assert(cv);

            if (cv1dir == 0) {
              if (cv->value < lastcv->value) {
                return false;
              }
            } else {
              if (cv->value > lastcv->value) {
                return false;
              }
            }

            if (cv->value >= minsell && cv->value < maxsell) {
              return true;
            }

            return false;
          };

          pStrategy->init(assetsName, indicatorName, avgtimes, onbuy, onsell,
                          invest);
          pStrategy->setStopLess(0.10);

          pStrategy->simulateTrading();

          tr2::PNL pnl;
          pWallet->buildPNL(pnl);

          char strname[1024];
          sprintf(strname, "%s.%s-%d[%.3f:%.3f].%d[%.3f:%.3f]", assetsName,
                  indicatorName, cv0dir, minbuy, maxbuy, cv1dir, minsell,
                  maxsell);

          // pnl.print(strname);
          // pStrategy->print();

          tr.name = strname;
          pnl.getTrainResult(tr);
          pStrategy->getTrainResult(tr);

          if (tr.totalReturn > minValidReturn) {
            auto fn1 = joinPath(outputPath, strname);
            fn1 += ".csv";

            pnl.saveCSV(fn1.c_str(), true);
          }

          lst.push_back(tr);

          delete pStrategy;
          delete pWallet;

          // auto ct = time(NULL);
          // printf("Progress is %.2f%% (%d/%d)\n",
          //        100.0f * curtimes / maxtimes, curtimes, maxtimes);

          // if (ct > st) {
          //   printf("current time is %d s, last time is %d s\n",
          //          (int)(ct - st),
          //          (int)((ct - st) * maxtimes / curtimes) - (int)(ct -
          //          st));
          // }
        }
      }
    }
  }
  // }
  // }

  // auto fn = joinPath(outputPath, "train.csv");

  // saveTrainResult(fn.c_str(), lst);

  return true;
}

// trainSingleIndicator2Ex -
// 单独一个indicator的策略，考虑方向，调用 _trainSingleIndicator2Ex
bool trainSingleIndicator2Ex(Exchange& exchange, const char* assetsName,
                             const char* indicatorName, const char* outputPath,
                             Money invest, int avgtimes,
                             IndicatorDataValue off0, IndicatorDataValue off1,
                             IndicatorDataValue off2,
                             IndicatorDataValue maxoff2, float minValidReturn) {
  IndicatorDataValue minval, maxval;
  if (!calcIndicatorRange(exchange, assetsName, indicatorName, avgtimes, minval,
                          maxval)) {
    return false;
  }

  TrainResultList lst;

  minval = scaleValue(minval, off0);
  maxval = scaleValue(maxval, off0);
  auto maxtimes = 0;
  int curtimes = 0;

  for (auto cv0 = minval; cv0 <= maxval; cv0 += off0) {
    for (auto cv0off = off2; cv0off <= maxoff2; cv0off += off2) {
      maxtimes++;
    }
  }

  auto st = time(NULL);

  for (auto cv0 = minval; cv0 <= maxval; cv0 += off0) {
    for (auto cv0off = off2; cv0off <= maxoff2; cv0off += off2) {
      _trainSingleIndicator2Ex(lst, exchange, assetsName, indicatorName,
                               outputPath, invest, avgtimes, off0, off1, off2,
                               maxoff2, minValidReturn, minval, maxval, cv0,
                               cv0off);

      curtimes++;

      auto ct = time(NULL);
      printf("Progress is %.2f%% (%d/%d)\n", 100.0f * curtimes / maxtimes,
             curtimes, maxtimes);

      if (ct > st) {
        printf("current time is %d s, last time is %d s\n", (int)(ct - st),
               (int)((ct - st) * maxtimes / curtimes) - (int)(ct - st));
      }
    }
  }

  auto fn = joinPath(outputPath, "train.csv");

  saveTrainResult(fn.c_str(), lst);

  return true;
}

CR2END