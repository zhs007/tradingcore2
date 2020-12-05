#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <tradingcore2/client/train2.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/protos/tradingdb2.grpc.pb.h>
#include <tradingcore2/train.h>
#include <tradingcore2/trdb2/client.h>
#include <tradingcore2/utils.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

CR2BEGIN

const tradingpb::Candle *getCandle(tradingpb::Candles *candles, int64_t ts) {
  if (candles->candles_size() <= 0) {
    return NULL;
  }

  const tradingpb::Candle *pCandle = &candles->candles(0);

  if (ts <= pCandle->ts()) {
    return pCandle;
  }

  for (auto i = 1; i < candles->candles_size(); ++i) {
    auto cc = candles->mutable_candles(i);
    if (ts == cc->ts()) {
      return cc;
    }

    if (ts < cc->ts()) {
      return pCandle;
    }

    pCandle = cc;
  }

  return pCandle;
}

int calcTradingDays4Year(const tradingpb::Candles &candles) {
  if (candles.candles_size() > 0) {
    tm stm, etm;
    timestamp2timeUTC(candles.candles(0).ts(), stm);
    timestamp2timeUTC(candles.candles(candles.candles_size() - 1).ts(), etm);

    int yoff = etm.tm_year - stm.tm_year;
    if (yoff <= 0) {
      return candles.candles_size();
    }

    int sday = 364 - stm.tm_yday;
    if (sday < 0) {
      sday = 0;
    }
    int eday = etm.tm_yday;

    float fy = (yoff - 1) + sday / 365.0f + eday / 365.0f;

    return candles.candles_size() / fy;
  }

  return 0;
}

CR2END