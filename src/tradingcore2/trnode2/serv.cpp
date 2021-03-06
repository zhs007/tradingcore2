#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <tradingcore2/autorun.h>
#include <tradingcore2/exchange/trdb2cnfunds.h>
#include <tradingcore2/exchangemgr.h>
#include <tradingcore2/pnl2.h>
#include <tradingcore2/protos/tradingnode2.grpc.pb.h>
#include <tradingcore2/spinlock.h>
#include <tradingcore2/strategymgr.h>
#include <tradingcore2/train.h>
#include <tradingcore2/trnode2/serv.h>
#include <tradingcore2/trnode2/utils.h>
#include <tradingcore2/utils.h>
#include <unistd.h>

#include <atomic>
#include <iostream>
#include <memory>
#include <string>

CR2BEGIN

void TradingNode2Impl::init(const Config& cfg) {
  m_pCfg = &cfg;

  long cpus = sysconf(_SC_NPROCESSORS_ONLN);  // get # of online cores
  if (cfg.taskNums == 0) {
    m_maxTaskNums = cpus;
  } else if (cfg.taskNums >= cpus) {
    m_maxTaskNums = cpus;
  } else if (cfg.taskNums > 0) {
    m_maxTaskNums = cfg.taskNums;
  } else if (cfg.taskNums < 0) {
    if (-cfg.taskNums < cpus) {
      m_maxTaskNums = cpus + cfg.taskNums;
    } else {
      m_maxTaskNums = 1;
    }
  } else {
    assert(false && "TradingNode2Impl::init() error");
  }

  LOG(INFO) << "max tasks num :" << m_maxTaskNums;
}

// getServerInfo - get server infomation
::grpc::Status TradingNode2Impl::getServerInfo(
    ::grpc::ServerContext* context,
    const ::tradingpb::RequestServerInfo* request,
    ::tradingpb::ReplyServerInfo* response) {
  assert(context != NULL);
  assert(request != NULL);
  assert(response != NULL);

  LOG(INFO) << "getServerInfo...";

  if (!isValidTokens(request, response, *m_pCfg)) {
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "invalid token");
  }

  auto ni = response->mutable_nodeinfo();
  ni->set_curtasks(this->m_curTaskNums);
  ni->set_maxtasks(this->m_maxTaskNums);

  LOG(INFO) << "current tasks num :" << (int)m_curTaskNums;
  LOG(INFO) << "max tasks num :" << m_maxTaskNums;

  return grpc::Status::OK;
}

// calcPNL - calcPNL
::grpc::Status TradingNode2Impl::_calcPNL(
    ::grpc::ServerContext* context, const ::tradingpb::RequestCalcPNL* request,
    ::tradingpb::ReplyCalcPNL* response, time_t ct) {
  assert(context != NULL);
  assert(request != NULL);
  assert(response != NULL);

  LOG(INFO) << "_calcPNL...";

  logProtobuf("calcPNL", *request);

  if (!isValidTokens(request, response, *m_pCfg)) {
    LOG(ERROR) << "_calcPNL:isValidTokens " << request->basicrequest().token();

    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "invalid token");
  }

  std::string mainMarket = "";
  for (auto i = 0; i < request->params().assets_size(); ++i) {
    auto ca = request->params().assets(i);
    if (mainMarket != "") {
      if (mainMarket != ca.market()) {
        return grpc::Status(grpc::StatusCode::UNKNOWN,
                            "I get a defferent market.");
      }
    } else {
      mainMarket = ca.market();
    }
  }

  auto exchange =
      tr2::ExchangeMgr::getSingleton()->newExchange(mainMarket.c_str());
  if (exchange == NULL) {
    LOG(ERROR) << "_calcPNL:getExchange " << mainMarket;

    return grpc::Status(grpc::StatusCode::UNKNOWN, "I can't get exchange");
  }

  if (request->params().startts() > 0 && request->params().endts() > 0) {
    if (request->params().startts() >= request->params().endts()) {
      LOG(ERROR) << "_calcPNL:invalid timestamp" << request->params().startts()
                 << " " << request->params().endts();

      return grpc::Status(grpc::StatusCode::UNKNOWN, "Invalid timestamp");
    }
  }

  std::string mainasset;
  for (auto i = 0; i < request->params().assets_size(); ++i) {
    auto ca = request->params().assets(i);
    exchange->loadData(ca.code().c_str(), request->params().startts(),
                       request->params().endts(), request->params().offset());

    if (i == request->params().mainassetindex()) {
      mainasset = ca.code();
    }
  }

  for (auto i = 0; i < request->params().candles_size(); ++i) {
    auto cs = request->params().candles(i);
    exchange->mergeCandles(&cs);
  }

  if (request->params().mainassetindex() < 0) {
    exchange->rebuildTimeStampList(NULL);
  } else {
    exchange->rebuildTimeStampList(mainasset.c_str());
  }

  LOG(ERROR) << "_calcPNL:rebuildTimeStampList ok!";

  auto pWallet = new tr2::Wallet(*exchange);

  auto sts = request->params().startts();
  if (sts <= 0) {
    sts = exchange->getFirstTimeStamp();
  }

  tr2::PNL2 pnl2;

  for (auto i = 0; i < request->params().strategies_size(); ++i) {
    auto cs = request->params().strategies(i);
    auto ret = CtrlConditionMgr::getSingleton()->isValidStrategy(cs);
    if (ret == -1) {
      return grpc::Status(grpc::StatusCode::UNKNOWN, "isValidStrategy");
    }

    auto strategy = tr2::StrategyMgr::getSingleton()->newStrategy(
        cs.name().c_str(), *pWallet, *exchange);
    if (strategy == NULL) {
      return grpc::Status(grpc::StatusCode::UNKNOWN, "non strategy");
    }

    strategy->init(cs);
    strategy->simulateTrading();

    strategy->buildIndicators(request->params(), pnl2);
  }

  LOG(ERROR) << "_calcPNL:simulateTrading ok!";

  // tr2::PNL2 pnl2;
  pWallet->buildPNL2(request->params(), pnl2);

  LOG(ERROR) << "_calcPNL:buildPNL2 ok!";

  auto pPNLData = response->add_pnl();
  pPNLData->CopyFrom(pnl2.m_data);

  auto ni = response->mutable_nodeinfo();
  ni->set_curtasks(this->m_curTaskNums - 1);
  ni->set_maxtasks(this->m_maxTaskNums);

  delete pWallet;
  tr2::ExchangeMgr::getSingleton()->deleteExchange(exchange);

  LOG(INFO) << "TradingNode2Impl::_calcPNL end.";

  auto et = std::time(0);

  response->set_runseconds(et - ct);

  return grpc::Status::OK;
}

// train - train
::grpc::Status TradingNode2Impl::calcPNL(
    ::grpc::ServerContext* context, const ::tradingpb::RequestCalcPNL* request,
    ::tradingpb::ReplyCalcPNL* response) {
  assert(context != NULL);
  assert(request != NULL);
  assert(response != NULL);

  auto ct = std::time(0);

  LOG(INFO) << "calcPNL...";

  if (m_pCfg->isLimitTasks) {
    if (m_curTaskNums >= m_maxTaskNums) {
      auto ni = response->mutable_nodeinfo();

      ni->set_curtasks(this->m_curTaskNums);
      ni->set_maxtasks(this->m_maxTaskNums);

      return grpc::Status::OK;
    }
  }

  AutoIncDec<std::atomic<int>> aid(&m_curTaskNums);

  return _calcPNL(context, request, response, ct);
}

void NodeServer2::run() {
  std::string server_address(m_pCfg->bindaddr.c_str());
  TradingNode2Impl service;

  service.init(*m_pCfg);

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  grpc::ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  // std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

CR2END