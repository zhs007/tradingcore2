#ifndef __TRADINGCORE2_TRNODE2_CLIENT_H__
#define __TRADINGCORE2_TRNODE2_CLIENT_H__

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <tradingcore2/autorun.h>
#include <tradingcore2/exchangemgr.h>
#include <tradingcore2/protos/tradingnode2.grpc.pb.h>
#include <tradingcore2/server.h>
#include <tradingcore2/spinlock.h>
#include <tradingcore2/train.h>
#include <unistd.h>

#include <atomic>
#include <iostream>
#include <memory>
#include <string>

CR2BEGIN

// FuncOnGetServerInfo - for NodeClient2.getServerInfo
typedef std::function<void(::grpc::Status,
                           const ::tradingpb::RequestServerInfo&,
                           const ::tradingpb::ReplyServerInfo&)>
    FuncOnGetServerInfo;

// FuncOnCalcPNL - for NodeClient2.calcPNL
typedef std::function<void(::grpc::Status, const ::tradingpb::RequestCalcPNL&,
                           const ::tradingpb::ReplyCalcPNL&)>
    FuncOnCalcPNL;

class NodeClient2 {
 public:
  explicit NodeClient2(std::shared_ptr<grpc::Channel> channel,
                       const char* token)
      : m_stub(::tradingpb::TradingNode2::NewStub(channel)),
        m_token(token),
        m_pAsyncThread(NULL),
        m_lastTasks(0) {}

 public:
  // getServerInfo
  void getServerInfo(FuncOnGetServerInfo onGetServerInfo);
  // calcPNL
  void clacPNL(const ::tradingpb::SimTradingParams& params,
               FuncOnCalcPNL onCalcPNL);

 public:
  void start();

  void waitStop();

 protected:
  // _getServerInfo
  void _getServerInfo(FuncOnGetServerInfo onGetServerInfo);

  // _calcPNL
  void _clacPNL(const ::tradingpb::SimTradingParams* pParams,
                FuncOnCalcPNL onCalcPNL);

 private:
  std::unique_ptr<::tradingpb::TradingNode2::Stub> m_stub;

  std::string m_token;
  std::thread* m_pAsyncThread;
  std::atomic<int> m_lastTasks;
};

CR2END

#endif  // __TRADINGCORE2_TRNODE2_CLIENT_H__