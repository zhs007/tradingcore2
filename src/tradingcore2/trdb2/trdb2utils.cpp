#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <tradingcore2/client/train2.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/protos/tradingdb2.grpc.pb.h>
#include <tradingcore2/tasksmgr.h>
#include <tradingcore2/train.h>
#include <tradingcore2/trdb2/client.h>
#include <tradingcore2/trdb2/workermgr.h>
#include <tradingcore2/utils.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

CR2BEGIN

// getCandles - get candles
bool getCandles(tradingpb::Candles &candles, const char *host,
                const char *token, const char *market, const char *symbol,
                std::vector<const char *> *pTags, int64_t tsStart,
                int64_t tsEnd, int32_t offset)
{
  candles.set_market(market);
  candles.set_symbol(symbol);
  // candles.set_tag(tag);

  auto stub = tradingpb::TradingDB2::NewStub(
      grpc::CreateChannel(host, grpc::InsecureChannelCredentials()));

  tradingpb::RequestGetCandles req;
  grpc::ClientContext context;
  tradingpb::ReplyGetCandles reply;

  auto brd = req.mutable_basicrequest();
  brd->set_token(token);

  req.set_market(market);
  req.set_symbol(symbol);
  req.set_tsstart(tsStart);
  req.set_tsend(tsEnd);
  req.set_offset(offset);

  if (pTags != NULL)
  {
    for (auto it = pTags->begin(); it != pTags->end(); ++it)
    {
      req.add_tags(*it);
    }
  }

  LOG(INFO) << "getCandles " << req.DebugString();

  std::unique_ptr<grpc::ClientReader<tradingpb::ReplyGetCandles>> reader(
      stub->getCandles(&context, req));

  while (reader->Read(&reply))
  {
    auto ccs = reply.candles();
    for (auto i = 0; i < ccs.candles_size(); ++i)
    {
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

  if (status.ok())
  {
    LOG(INFO) << "getCandles " << market << "." << symbol << " ok. "
              << candles.candles_size();

    return true;
  }

  LOG(ERROR) << "getCandles " << market << "." << symbol << " failed. "
             << status.error_message();

  return false;
}

// getSymbols - get symbols
bool getSymbols(const char *host, const char *token, const char *market,
                std::vector<const char *> *pSymbols,
                FuncOnSymbol funcOnSymbol)
{
  auto stub = tradingpb::TradingDB2::NewStub(
      grpc::CreateChannel(host, grpc::InsecureChannelCredentials()));

  tradingpb::RequestGetSymbols req;
  grpc::ClientContext context;
  tradingpb::ReplyGetSymbol reply;

  auto brd = req.mutable_basicrequest();
  brd->set_token(token);

  req.set_market(market);

  if (pSymbols != NULL)
  {
    for (auto it = pSymbols->begin(); it != pSymbols->end(); ++it)
    {
      req.add_symbols(*it);
    }
  }

  std::unique_ptr<grpc::ClientReader<tradingpb::ReplyGetSymbol>> reader(
      stub->getSymbols(&context, req));

  while (reader->Read(&reply))
  {
    auto cs = reply.symbol();

    funcOnSymbol(cs);
  }

  grpc::Status status = reader->Finish();

  if (status.ok())
  {
    return true;
  }

  return false;
}

// updSymbol - update symbol
bool updSymbol(const char *host, const char *token, tradingpb::SymbolInfo &si)
{
  auto stub = tradingpb::TradingDB2::NewStub(
      grpc::CreateChannel(host, grpc::InsecureChannelCredentials()));
  grpc::ClientContext context;
  tradingpb::RequestUpdSymbol req;
  tradingpb::ReplyUpdSymbol reply;

  auto brd = req.mutable_basicrequest();
  brd->set_token(token);

  *req.mutable_symbol() = si;

  auto status = stub->updSymbol(&context, req, &reply);
  if (status.ok())
  {
    if (reply.isok())
    {
      return true;
    }
  }

  return false;
}

// reqTasks - request tasks
void reqTasks(const char *host, const char *token, WorkerMgr *mgrWorker)
{
  auto stub = tradingpb::TradingDB2::NewStub(
      grpc::CreateChannel(host, grpc::InsecureChannelCredentials()));

  grpc::ClientContext context;
  std::shared_ptr<grpc::ClientReaderWriter<tradingpb::RequestTradingTask,
                                           tradingpb::ReplyTradingTask>>
      stream(stub->reqTradingTask3(&context));

  std::mutex *pmtx = new std::mutex();
  tradingpb::RequestTradingTask req;
  tradingpb::ReplyTradingTask reply;
  int tasknums = 0;
  bool isEnd = false;
  int firstTaskNums = 0;
  // int workernums = 0;

  auto brd = req.mutable_basicrequest();
  brd->set_token(token);

  stream->Write(req);

  while (stream->Read(&reply))
  {
    if (isEnd)
    {
      continue;
    }

    if (!reply.has_params())
    {
      while (mgrWorker->hasRunningWorker())
      {
        std::this_thread::sleep_for(std::chrono::seconds(5));
      }

      if (tasknums > 0)
      {
        LOG(INFO) << "reqTasks all worker end.";
      }

      while (TasksMgr::getSingleton()->getCurTaskNums() > 0)
      {
        std::this_thread::sleep_for(std::chrono::seconds(5));
      }

      {
        std::lock_guard<std::mutex> lock(*pmtx);

        stream->WritesDone();
      }

      isEnd = true;

      if (tasknums > 0)
      {
        LOG(INFO) << "reqTasks " << tasknums << " end. ";
      }

      // break;
    }
    else
    {
      if (mgrWorker->canFinish())
      {
        LOG(INFO) << "reqTasks finished.";

        std::string task;
        reply.params().SerializeToString(&task);

        tradingpb::RequestTradingTask rtt;

        auto brd = rtt.mutable_basicrequest();
        brd->set_token(token);

        auto mr = rtt.mutable_result();
        mr->set_task(task);
        mr->set_err("finished.");

        {
          std::lock_guard<std::mutex> lock(*pmtx);
          stream->Write(rtt);
        }

        while (mgrWorker->hasRunningWorker())
        {
          std::this_thread::sleep_for(std::chrono::seconds(5));
        }

        if (tasknums > 0)
        {
          LOG(INFO) << "reqTasks all worker end.";
        }

        while (TasksMgr::getSingleton()->getCurTaskNums() > 0)
        {
          std::this_thread::sleep_for(std::chrono::seconds(5));
        }

        {
          std::lock_guard<std::mutex> lock(*pmtx);

          stream->WritesDone();
        }

        isEnd = true;

        if (tasknums > 0)
        {
          LOG(INFO) << "reqTasks " << tasknums << " end. ";
        }
      }
      else if (!mgrWorker->hasFreeWorker())
      {
        LOG(INFO) << "reqTasks non-worker.";

        std::string task;
        reply.params().SerializeToString(&task);

        tradingpb::RequestTradingTask rtt;

        auto brd = rtt.mutable_basicrequest();
        brd->set_token(token);

        auto mr = rtt.mutable_result();
        mr->set_task(task);
        mr->set_err("non-worker");

        {
          std::lock_guard<std::mutex> lock(*pmtx);
          stream->Write(rtt);
        }
      }
      else
      {
        ++tasknums;
        ++firstTaskNums;

        ::tradingpb::SimTradingParams *pParams = reply.params().New();
        pParams->CopyFrom(reply.params());

        auto workerID = mgrWorker->newWorkerID();

        std::thread *pWorker = new std::thread([workerID, mgrWorker, stream,
                                                pParams, token, pmtx]()
                                               {
                                                 ::tradingpb::PNLData pnldata;
                                                 auto status = TasksMgr::getSingleton()->runTask(pParams, &pnldata);

                                                 if (status.ok())
                                                 {
                                                   std::string task;
                                                   pParams->SerializeToString(&task);

                                                   tradingpb::RequestTradingTask rtt;

                                                   auto brd = rtt.mutable_basicrequest();
                                                   brd->set_token(token);

                                                   auto mr = rtt.mutable_result();
                                                   mr->set_task(task);
                                                   auto pnl = mr->mutable_pnl();
                                                   pnl->CopyFrom(pnldata);

                                                   {
                                                     std::lock_guard<std::mutex> lock(*pmtx);
                                                     stream->Write(rtt);
                                                   }
                                                 }
                                                 else
                                                 {
                                                   std::string task;
                                                   pParams->SerializeToString(&task);

                                                   tradingpb::RequestTradingTask rtt;

                                                   auto brd = rtt.mutable_basicrequest();
                                                   brd->set_token(token);

                                                   auto mr = rtt.mutable_result();
                                                   mr->set_task(task);
                                                   mr->set_err(status.error_message());

                                                   {
                                                     std::lock_guard<std::mutex> lock(*pmtx);
                                                     stream->Write(rtt);
                                                   }
                                                 }

                                                 mgrWorker->delWorker(workerID);
                                               });

        mgrWorker->insWorker(workerID, pWorker);

        if (firstTaskNums < mgrWorker->getMaxWorkerNums())
        {
          std::lock_guard<std::mutex> lock(*pmtx);
          stream->Write(req);
        }
      }
    }
  }

  auto status = stream->Finish();
  if (!status.ok())
  {
    LOG(ERROR) << "reqTasks " << tasknums << " failed. "
               << status.error_message();
  }

  if (tasknums > 0)
  {
    LOG(INFO) << "reqTasks " << tasknums << " finished. ";
  }
}

CR2END