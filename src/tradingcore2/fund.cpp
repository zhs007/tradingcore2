#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <tradingcore2/client/train2.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/fund.h>
#include <tradingcore2/pnl.h>
#include <tradingcore2/protos/tradingdb2.grpc.pb.h>
#include <tradingcore2/train.h>
#include <tradingcore2/trdb2/client.h>
#include <tradingcore2/utils.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

CR2BEGIN

// analysisFund - analysis fund
bool analysisFund(PNL& pnl, const Exchange& exchange,
                  const tradingpb::SymbolInfo& si,
                  const tradingpb::Candles& candles) {
  pnl.release();
  pnl.initWithCandles(candles);
  pnl.calcValidDataPer(si, exchange);
  pnl.onBuildEnd(exchange);

  return true;
}

CR2END