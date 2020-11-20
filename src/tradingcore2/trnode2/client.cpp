#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <tradingcore2/autorun.h>
#include <tradingcore2/exchange/trdb2cnfunds.h>
#include <tradingcore2/exchangemgr.h>
#include <tradingcore2/pnl2.h>
#include <tradingcore2/protos/tradingnode2.grpc.pb.h>
#include <tradingcore2/spinlock.h>
#include <tradingcore2/train.h>
#include <tradingcore2/trnode2/client.h>
#include <tradingcore2/trnode2/utils.h>
#include <unistd.h>

#include <atomic>
#include <iostream>
#include <memory>
#include <string>

CR2BEGIN

// getServerInfo
void NodeClient2::getServerInfo(FuncOnGetServerInfo onGetServerInfo) {
  this->m_lastTasks++;

  std::thread(&NodeClient2::_getServerInfo, this, onGetServerInfo);
}

void NodeClient2::_getServerInfo(FuncOnGetServerInfo onGetServerInfo) {
  ::tradingpb::RequestServerInfo req;

  auto basicReq = req.mutable_basicrequest();
  basicReq->set_token(this->m_token.c_str());

  grpc::ClientContext ctx;
  ::tradingpb::ReplyServerInfo res;
  auto reply = this->m_stub->getServerInfo(&ctx, req, &res);
  if (onGetServerInfo != NULL) {
    onGetServerInfo(reply, req, res);
  }

  this->m_lastTasks--;
}

// calcPNL
void NodeClient2::clacPNL(const ::tradingpb::SimTradingParams& params,
                          FuncOnCalcPNL onCalcPNL) {
  this->m_lastTasks++;

  std::thread(&NodeClient2::_clacPNL, this, &params, onCalcPNL);
}

// _calcPNL
void NodeClient2::_clacPNL(const ::tradingpb::SimTradingParams* pParams,
                           FuncOnCalcPNL onCalcPNL) {
  ::tradingpb::RequestCalcPNL req;

  auto basicReq = req.mutable_basicrequest();
  basicReq->set_token(this->m_token.c_str());

  auto params = req.mutable_params();
  params->CopyFrom(*pParams);

  grpc::ClientContext ctx;
  ::tradingpb::ReplyCalcPNL res;
  auto reply = this->m_stub->calcPNL(&ctx, req, &res);
  if (onCalcPNL != NULL) {
    LOG(INFO) << "NodeClient2::_clacPNL " << reply.error_code();

    onCalcPNL(reply, req, res);
  }

  this->m_lastTasks--;
}

void NodeClient2::start() {}

void NodeClient2::waitStop() {
  while (this->m_lastTasks > 0) {
    sleep(1);
  }
}

// getServerInfo
::grpc::Status NodeClient2::getServerInfo(::tradingpb::ReplyServerInfo& res) {
  ::tradingpb::RequestServerInfo req;

  auto basicReq = req.mutable_basicrequest();
  basicReq->set_token(this->m_token.c_str());

  grpc::ClientContext ctx;
  return this->m_stub->getServerInfo(&ctx, req, &res);
}

// calcPNL
::grpc::Status NodeClient2::clacPNL(const ::tradingpb::SimTradingParams& params,
                                    ::tradingpb::ReplyCalcPNL& res) {
  ::tradingpb::RequestCalcPNL req;

  auto basicReq = req.mutable_basicrequest();
  basicReq->set_token(this->m_token.c_str());

  auto p = req.mutable_params();
  p->CopyFrom(params);

  grpc::ClientContext ctx;
  return this->m_stub->calcPNL(&ctx, req, &res);
}

CR2END