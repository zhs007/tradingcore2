#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <tradingcore2/autorun.h>
#include <tradingcore2/exchangemgr.h>
#include <tradingcore2/protos/tradingnode2.grpc.pb.h>
#include <tradingcore2/spinlock.h>
#include <tradingcore2/train.h>
#include <tradingcore2/trnode2/serv.h>
#include <tradingcore2/trnode2/utils.h>
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

  response->set_curtasks(this->m_curTaskNums);
  response->set_maxtasks(this->m_maxTaskNums);

  LOG(INFO) << "current tasks num :" << (int)m_curTaskNums;
  LOG(INFO) << "max tasks num :" << m_maxTaskNums;

  return grpc::Status::OK;
}

// calcPNL - calcPNL
::grpc::Status TradingNode2Impl::_calcPNL(
    ::grpc::ServerContext* context, const ::tradingpb::RequestCalcPNL* request,
    ::tradingpb::ReplyCalcPNL* response) {
  assert(context != NULL);
  assert(request != NULL);
  assert(response != NULL);

  return grpc::Status::OK;
}

// train - train
::grpc::Status TradingNode2Impl::calcPNL(
    ::grpc::ServerContext* context, const ::tradingpb::RequestCalcPNL* request,
    ::tradingpb::ReplyCalcPNL* response) {
  assert(context != NULL);
  assert(request != NULL);
  assert(response != NULL);

  if (m_pCfg->isLimitTasks) {
    SpinLockMaxVal<int> lock(&m_curTaskNums, m_maxTaskNums);

    return _calcPNL(context, request, response);
  }

  AutoIncDec<std::atomic<int>> aid(&m_curTaskNums);

  return _calcPNL(context, request, response);
}

// calcPNL - calcPNL
::grpc::Status _calcPNL(::grpc::ServerContext* context,
                        const ::tradingpb::RequestCalcPNL* request,
                        ::tradingpb::ReplyCalcPNL* response) {
  return grpc::Status::OK;
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