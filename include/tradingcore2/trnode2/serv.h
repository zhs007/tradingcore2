#ifndef __TRADINGCORE2_TRNODE2_SERV_H__
#define __TRADINGCORE2_TRNODE2_SERV_H__

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

// Logic and data behind the server's behavior.
class TradingNode2Impl final : public tradingpb::TradingNode2::Service {
 public:
  TradingNode2Impl() : m_maxTaskNums(0), m_curTaskNums(0), m_pCfg(NULL) {}

 public:
  void init(const Config& cfg);

 public:
  // getServerInfo - get server infomation
  virtual ::grpc::Status getServerInfo(
      ::grpc::ServerContext* context,
      const ::tradingpb::RequestServerInfo* request,
      ::tradingpb::ReplyServerInfo* response) override;

  // calcPNL - calcPNL
  virtual ::grpc::Status calcPNL(::grpc::ServerContext* context,
                                 const ::tradingpb::RequestCalcPNL* request,
                                 ::tradingpb::ReplyCalcPNL* response) override;

 private:
  // calcPNL - calcPNL
  ::grpc::Status _calcPNL(::grpc::ServerContext* context,
                          const ::tradingpb::RequestCalcPNL* request,
                          ::tradingpb::ReplyCalcPNL* response);

 private:
  int m_maxTaskNums;
  std::atomic<int> m_curTaskNums;
  const Config* m_pCfg;
};

class NodeServer2 final : public Server {
 public:
  NodeServer2(const Config& cfg) : m_pCfg(&cfg) {}
  virtual ~NodeServer2() {}

 public:
  virtual void run() override;

 private:
  const Config* m_pCfg;
};

inline Server* newNodeServer2(const Config& cfg) {
  return new NodeServer2(cfg);
}

CR2END

#endif  // __TRADINGCORE2_TRNODE2_SERV_H__