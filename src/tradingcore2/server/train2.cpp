#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <tradingcore2/exchangemgr.h>
#include <tradingcore2/server/train2.h>
#include <tradingcore2/train.h>

#include <iostream>
#include <memory>
#include <string>

#include "../proto/tradingcore2.grpc.pb.h"
#include "utils.h"

CR2BEGIN

// Logic and data behind the server's behavior.
class TrainService2Impl final
    : public tradingcore2pb::TradingCore2Service::Service {
  // getServerInfo - get server infomation
  virtual ::grpc::Status getServerInfo(
      ::grpc::ServerContext* context,
      const ::tradingcore2pb::RequestServerInfo* request,
      ::tradingcore2pb::ReplyServerInfo* response) {
    assert(context != NULL);
    assert(request != NULL);
    assert(response != NULL);

    return grpc::Status::OK;
  }

  // train - train
  virtual ::grpc::Status train(::grpc::ServerContext* context,
                               const ::tradingcore2pb::RequestTrain* request,
                               ::tradingcore2pb::ReplyTrain* response) {
    assert(context != NULL);
    assert(request != NULL);
    assert(response != NULL);

    auto req = request->train();
    auto res = response->mutable_train();

    auto mgr = ExchangeMgr::getSingleton();
    auto exchange = mgr->getExchange(req.exchangename().c_str());
    if (exchange == NULL) {
      setResponse_ErrorCode(response, tradingcore2pb::ERR_NOEXCHANGE);

      return grpc::Status::OK;
    }

    auto datasize = exchange->getDataLength(req.assetsname().c_str());
    if (datasize <= 0) {
      setResponse_ErrorCode(response, tradingcore2pb::ERR_NOASSETS);

      return grpc::Status::OK;
    }

    if (req.has_si2()) {
      auto si2 = req.si2();

      TrainResultList lst;
      _trainSingleIndicator2Ex(
          lst, *exchange, req.assetsname().c_str(), si2.indicatorname().c_str(),
          req.outputpath().c_str(), req.invest(), si2.avgtimes(), si2.off0(),
          si2.off1(), si2.off2(), si2.maxoff2(), req.minvalidreturn(),
          si2.minval(), si2.maxval(), si2.cv0(), si2.cv0off());

      for (auto it = lst.begin(); it != lst.end(); ++it) {
        auto tr = res->add_nodes();

        tr->set_maxdrawdown(it->maxDrawDown);
        tr->set_sharpe(it->sharpe);
        tr->set_annualizedreturns(it->annualizedReturns);
        tr->set_annualizedvolatility(it->annualizedVolatility);
        tr->set_totalreturns(it->totalReturn);
        tr->set_tradingtimes(it->tradingNums);
        tr->set_stoplosstimes(it->stoplossNums);
        tr->set_winrate(it->winRate);
        tr->set_failtimes(it->failNums);
        tr->set_name(it->name.c_str());
      }
    } else {
      setResponse_ErrorCode(response, tradingcore2pb::ERR_NOTRAINPARAM);

      return grpc::Status::OK;
    }

    return grpc::Status::OK;
  }
};

class TrainServer2 final : public Server {
 public:
  TrainServer2(const char* strBindAddr) : m_bindAddr(strBindAddr) {}
  virtual ~TrainServer2() {}

 public:
  virtual void run() override {
    std::string server_address(m_bindAddr.c_str());
    TrainService2Impl service;

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

 private:
  std::string m_bindAddr;
};

Server* newTrainServer2(const char* strBindAddr) {
  return new TrainServer2(strBindAddr);
}

CR2END