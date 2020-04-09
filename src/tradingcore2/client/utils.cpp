#include "utils.h"

CR2BEGIN

void setTradeHistory2TrainResult(TrainResult& tr,
                                 const tradingcore2pb::TrainNodeResult& tnr) {
  tr.maxDrawDown = tnr.maxdrawdown();
  tr.sharpe = tnr.sharpe();
  tr.annualizedReturns = tnr.annualizedreturns();
  tr.annualizedVolatility = tnr.annualizedvolatility();
  tr.totalReturn = tnr.totalreturns();
  tr.tradingNums = tnr.tradingtimes();
  tr.stoplossNums = tnr.stoplosstimes();
  tr.winRate = tnr.winrate();
  tr.failNums = tnr.failtimes();
  tr.name = tnr.name();

  if (tnr.history_size() > 0) {
    tradingcore2pb::TrainNodeResult* pTNR =
        new tradingcore2pb::TrainNodeResult();

    for (auto i = 0; i < tnr.history_size(); ++i) {
      auto ch = pTNR->add_history();

      *ch = tnr.history(i);
    }

    tr.data = pTNR;
  } else {
    tr.data = NULL;
  }
}

CR2END