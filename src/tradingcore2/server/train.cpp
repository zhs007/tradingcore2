
#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <tradingcore2/server/train.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "../proto/tradingcore2.grpc.pb.h"

CR2BEGIN

// using grpc::Server;
// using grpc::ServerAsyncResponseWriter;
// using grpc::ServerBuilder;
// using grpc::ServerCompletionQueue;
// using grpc::ServerContext;
// using grpc::Status;
// using tradingcore2pb::TradingCore2Service;
// using tradingcore2pb::TrainData;
// using tradingcore2pb::TrainResult;

class TrainServer : public Server {
 public:
  TrainServer(const char* strBindAddr) : m_bindAddr(strBindAddr) {}
  virtual ~TrainServer() {
    m_server->Shutdown();
    // Always shutdown the completion queue after the server.
    m_cq->Shutdown();
  }

  // There is no shutdown handling in this code.
  virtual void run() {
    std::string server_address(m_bindAddr.c_str());

    grpc::ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service_" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *asynchronous* service.
    builder.RegisterService(&m_service);
    // Get hold of the completion queue used for the asynchronous communication
    // with the gRPC runtime.
    m_cq = builder.AddCompletionQueue();
    // Finally assemble the server.
    m_server = builder.BuildAndStart();
    // std::cout << "Server listening on " << server_address << std::endl;

    // Proceed to the server's main loop.
    handleRpcs();
  }

 private:
  // Class encompasing the state and logic needed to serve a request.
  class CallData {
   public:
    // Take in the "service" instance (in this case representing an asynchronous
    // server) and the completion queue "cq" used for asynchronous communication
    // with the gRPC runtime.
    CallData(tradingcore2pb::TradingCore2Service::AsyncService* service,
             grpc::ServerCompletionQueue* cq)
        : m_service(service), m_cq(cq), m_responder(&m_ctx), m_status(CREATE) {
      // Invoke the serving logic right away.
      proceed();
    }

    void proceed() {
      if (m_status == CREATE) {
        // Make this instance progress to the PROCESS state.
        m_status = PROCESS;

        // As part of the initial CREATE state, we *request* that the system
        // start processing SayHello requests. In this request, "this" acts are
        // the tag uniquely identifying the request (so that different CallData
        // instances can serve different requests concurrently), in this case
        // the memory address of this CallData instance.
        m_service->Requesttrain(&m_ctx, &m_request, &m_responder, m_cq, m_cq,
                                this);
      } else if (m_status == PROCESS) {
        // Spawn a new CallData instance to serve new clients while we process
        // the one for this CallData. The instance will deallocate itself as
        // part of its FINISH state.
        new CallData(m_service, m_cq);

        // The actual processing.
        // std::string prefix("Hello ");
        // reply_.set_message(prefix + request_.name());
        m_reply.set_trainid(m_request.trainid());

        // And we are done! Let the gRPC runtime know we've finished, using the
        // memory address of this instance as the uniquely identifying tag for
        // the event.
        m_status = FINISH;
        m_responder.Finish(m_reply, grpc::Status::OK, this);
      } else {
        GPR_ASSERT(m_status == FINISH);
        // Once in the FINISH state, deallocate ourselves (CallData).
        delete this;
      }
    }

   private:
    // The means of communication with the gRPC runtime for an asynchronous
    // server.
    tradingcore2pb::TradingCore2Service::AsyncService* m_service;
    // The producer-consumer queue where for asynchronous server notifications.
    grpc::ServerCompletionQueue* m_cq;
    // Context for the rpc, allowing to tweak aspects of it such as the use
    // of compression, authentication, as well as to send metadata back to the
    // client.
    grpc::ServerContext m_ctx;

    // What we get from the client.
    tradingcore2pb::TrainData m_request;
    // What we send back to the client.
    tradingcore2pb::TrainResult m_reply;

    // The means to get back to the client.
    grpc::ServerAsyncResponseWriter<tradingcore2pb::TrainResult> m_responder;

    // Let's implement a tiny state machine with the following states.
    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus m_status;  // The current serving state.
  };

  // This can be run in multiple threads if needed.
  void handleRpcs() {
    // Spawn a new CallData instance to serve new clients.
    new CallData(&m_service, m_cq.get());

    void* tag;  // uniquely identifies a request.
    bool ok;
    while (true) {
      // Block waiting to read the next event from the completion queue. The
      // event is uniquely identified by its tag, which in this case is the
      // memory address of a CallData instance.
      // The return value of Next should always be checked. This return value
      // tells us whether there is any kind of event or cq_ is shutting down.
      GPR_ASSERT(m_cq->Next(&tag, &ok));
      GPR_ASSERT(ok);
      static_cast<CallData*>(tag)->proceed();
    }
  }

 private:
  std::unique_ptr<grpc::ServerCompletionQueue> m_cq;
  tradingcore2pb::TradingCore2Service::AsyncService m_service;
  std::unique_ptr<grpc::Server> m_server;

  std::string m_bindAddr;
};

Server* newTrainServer(const char* strBindAddr) {
  return new TrainServer(strBindAddr);
}

CR2END