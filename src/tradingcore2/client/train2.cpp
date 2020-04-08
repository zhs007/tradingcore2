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

#include "../proto/tradingcore2.grpc.pb.h"

// using grpc::Channel;
// using grpc::ClientAsyncResponseReader;
// using grpc::ClientContext;
// using grpc::CompletionQueue;
// using grpc::Status;
// using helloworld::Greeter;
// using helloworld::HelloReply;
// using helloworld::HelloRequest;

CR2BEGIN

class TrainClient2 {
 public:
  explicit TrainClient2(std::shared_ptr<grpc::Channel> channel)
      : m_stub(tradingcore2pb::TradingCore2Service::NewStub(channel)) {}

  // Assembles the client's payload and sends it to the server.
  void train(const char* exchangeName, const char* assetsName,
             const char* indicatorName, const char* outputPath, Money invest,
             int avgtimes, IndicatorDataValue off0, IndicatorDataValue off1,
             IndicatorDataValue off2, IndicatorDataValue maxoff2,
             float minValidReturn, IndicatorDataValue minval,
             IndicatorDataValue maxval, IndicatorDataValue cv0,
             IndicatorDataValue cv0off) {
    // Data we are sending to the server.
    // HelloRequest request;
    tradingcore2pb::RequestTrain request;
    auto req = request.mutable_train();

    req->set_exchangename(exchangeName);
    req->set_assetsname(assetsName);
    req->set_invest(invest);
    req->set_outputpath(outputPath);
    req->set_minvalidreturn(minValidReturn);

    auto si2 = req->mutable_si2();
    si2->set_indicatorname(indicatorName);
    si2->set_avgtimes(avgtimes);
    si2->set_off0(off0);
    si2->set_off1(off1);
    si2->set_off2(off2);
    si2->set_maxoff2(maxoff2);
    si2->set_minval(minval);
    si2->set_maxval(maxval);
    si2->set_cv0(cv0);
    si2->set_cv0off(cv0off);

    // request.set_name(user);

    // Call object to store rpc data
    AsyncClientCall* call = new AsyncClientCall;

    // stub_->PrepareAsyncSayHello() creates an RPC object, returning
    // an instance to store in "call" but does not actually start the RPC
    // Because we are using the asynchronous API, we need to hold on to
    // the "call" instance in order to get updates on the ongoing RPC.
    call->response_reader =
        m_stub->PrepareAsynctrain(&call->context, request, &m_cq);

    // StartCall initiates the RPC call
    call->response_reader->StartCall();

    // Request that, upon completion of the RPC, "reply" be updated with the
    // server's response; "status" with the indication of whether the operation
    // was successful. Tag the request with the memory address of the call
    // object.
    call->response_reader->Finish(&call->reply, &call->status, (void*)call);
  }

  // Loop while listening for completed responses.
  // Prints out the response from the server.
  void AsyncCompleteRpc() {
    void* got_tag;
    bool ok = false;

    // Block until the next result is available in the completion queue "cq".
    while (m_cq.Next(&got_tag, &ok)) {
      // The tag in this example is the memory location of the call object
      AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);

      // Verify that the request was completed successfully. Note that "ok"
      // corresponds solely to the request for updates introduced by Finish().
      GPR_ASSERT(ok);

      if (call->status.ok())
        std::cout << "RPC received: " << call->reply.train().nodes_size()
                  << std::endl;
      else
        std::cout << "RPC failed" << std::endl;

      // Once we're complete, deallocate the call object.
      delete call;
    }
  }

 private:
  // struct for keeping state and data information
  struct AsyncClientCall {
    // Container for the data we expect from the server.
    // HelloReply reply;
    tradingcore2pb::ReplyTrain reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    grpc::ClientContext context;

    // Storage for the status of the RPC upon completion.
    grpc::Status status;

    std::unique_ptr<
        grpc::ClientAsyncResponseReader<tradingcore2pb::ReplyTrain>>
        response_reader;
  };

  // Out of the passed in Channel comes the stub, stored here, our view of the
  // server's exposed services.
  std::unique_ptr<tradingcore2pb::TradingCore2Service::Stub> m_stub;

  // The producer-consumer queue we use to communicate asynchronously with the
  // gRPC runtime.
  grpc::CompletionQueue m_cq;
};

// startTrainSingleIndicator2Ex - 单独一个indicator的策略，考虑方向，grpc调用
bool startTrainSingleIndicator2Ex(
    const char* host, Exchange& exchange, const char* assetsName,
    const char* indicatorName, const char* outputPath, Money invest,
    int avgtimes, IndicatorDataValue off0, IndicatorDataValue off1,
    IndicatorDataValue off2, IndicatorDataValue maxoff2, float minValidReturn) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50051). We indicate that the channel isn't authenticated
  // (use of InsecureChannelCredentials()).
  TrainClient2 client(
      grpc::CreateChannel(host, grpc::InsecureChannelCredentials()));

  // Spawn reader thread that loops indefinitely
  std::thread thread_ = std::thread(&TrainClient2::AsyncCompleteRpc, &client);

  IndicatorDataValue minval, maxval;
  if (!calcIndicatorRange(exchange, assetsName, indicatorName, avgtimes, minval,
                          maxval)) {
    return false;
  }

  TrainResultList lst;

  minval = scaleValue(minval, off0);
  maxval = scaleValue(maxval, off0);
  auto maxtimes = 0;
  int curtimes = 0;

  for (auto cv0 = minval; cv0 <= maxval; cv0 += off0) {
    for (auto cv0off = off2; cv0off <= maxoff2; cv0off += off2) {
      maxtimes++;
    }
  }

  auto st = time(NULL);

  for (auto cv0 = minval; cv0 <= maxval; cv0 += off0) {
    for (auto cv0off = off2; cv0off <= maxoff2; cv0off += off2) {
      client.train(exchange.getTypeName(), assetsName, indicatorName,
                   outputPath, invest, avgtimes, off0, off1, off2, maxoff2,
                   minValidReturn, minval, maxval, cv0, cv0off);

      curtimes++;

      auto ct = time(NULL);
      printf("Progress is %.2f%% (%d/%d)\n", 100.0f * curtimes / maxtimes,
             curtimes, maxtimes);

      if (ct > st) {
        printf("current time is %d s, last time is %d s\n", (int)(ct - st),
               (int)((ct - st) * maxtimes / curtimes) - (int)(ct - st));
      }
    }
  }

  //   for (int i = 0; i < 100; i++) {
  //     // std::string user("world " + std::to_string(i));
  //     client.train(user);  // The actual RPC call!
  //   }

  //   std::cout << "Press control-c to quit" << std::endl << std::endl;
  thread_.join();  // blocks forever

  return true;
}

// int main(int argc, char** argv) {
//   // Instantiate the client. It requires a channel, out of which the actual
//   RPCs
//   // are created. This channel models a connection to an endpoint (in this
//   case,
//   // localhost at port 50051). We indicate that the channel isn't
//   authenticated
//   // (use of InsecureChannelCredentials()).
//   GreeterClient greeter(grpc::CreateChannel(
//       "localhost:50051", grpc::InsecureChannelCredentials()));

//   // Spawn reader thread that loops indefinitely
//   std::thread thread_ = std::thread(&GreeterClient::AsyncCompleteRpc,
//   &greeter);

//   for (int i = 0; i < 100; i++) {
//     std::string user("world " + std::to_string(i));
//     greeter.SayHello(user);  // The actual RPC call!
//   }

//   std::cout << "Press control-c to quit" << std::endl << std::endl;
//   thread_.join();  // blocks forever

//   return 0;
// }

CR2END