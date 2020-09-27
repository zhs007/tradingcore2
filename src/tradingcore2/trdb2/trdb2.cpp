#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <tradingcore2/client/train2.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/train.h>
#include <tradingcore2/utils.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "../proto/tradingdb2.grpc.pb.h"

CR2BEGIN

// getCandles - get candles
bool getCandles(tradingdb2pb::Candles &candles, const char *host,
                const char *token, const char *market, const char *symbol,
                const char *tag) {
  candles.set_market(market);
  candles.set_symbol(symbol);
  candles.set_tag(tag);

  auto stub = tradingdb2pb::TradingDB2Service::NewStub(
      grpc::CreateChannel(host, grpc::InsecureChannelCredentials()));

  tradingdb2pb::RequestGetCandles req;
  grpc::ClientContext context;
  tradingdb2pb::ReplyGetCandles reply;

  req.set_token(token);
  req.set_market(market);
  req.set_symbol(symbol);
  req.set_tag(tag);

  std::unique_ptr<grpc::ClientReader<tradingdb2pb::ReplyGetCandles>> reader(
      stub->getCandles(&context, req));

  while (reader->Read(&reply)) {
    auto ccs = reply.candles();
    for (auto i = 0; i < ccs.candles_size(); ++i) {
      auto csc = ccs.candles(i);
      auto cc = candles.add_candles();

      cc->set_ts(csc.ts());
      cc->set_open(csc.open());
      cc->set_close(csc.close());
      cc->set_high(csc.high());
      cc->set_low(csc.low());
      cc->set_volume(csc.volume());
      cc->set_openinterest(csc.openinterest());
      cc->set_trades(csc.trades());
      cc->set_vwap(csc.vwap());
      cc->set_lastsize(csc.lastsize());
      cc->set_turnover(csc.turnover());
      cc->set_homenotional(csc.homenotional());
      cc->set_foreignnotional(csc.foreignnotional());
    }
  }

  grpc::Status status = reader->Finish();

  if (status.ok()) {
    return true;
  }

  return false;
}

CR2END