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

CR2END