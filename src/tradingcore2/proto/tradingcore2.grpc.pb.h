// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: tradingcore2.proto
#ifndef GRPC_tradingcore2_2eproto__INCLUDED
#define GRPC_tradingcore2_2eproto__INCLUDED

#include "tradingcore2.pb.h"

#include <functional>
#include <grpc/impl/codegen/port_platform.h>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace tradingcore2pb {

// TradingCore2Service - TradingCore2 service
class TradingCore2Service final {
 public:
  static constexpr char const* service_full_name() {
    return "tradingcore2pb.TradingCore2Service";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    // train - train
    virtual ::grpc::Status train(::grpc::ClientContext* context, const ::tradingcore2pb::TrainData& request, ::tradingcore2pb::TrainResult* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tradingcore2pb::TrainResult>> Asynctrain(::grpc::ClientContext* context, const ::tradingcore2pb::TrainData& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tradingcore2pb::TrainResult>>(AsynctrainRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tradingcore2pb::TrainResult>> PrepareAsynctrain(::grpc::ClientContext* context, const ::tradingcore2pb::TrainData& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tradingcore2pb::TrainResult>>(PrepareAsynctrainRaw(context, request, cq));
    }
    class experimental_async_interface {
     public:
      virtual ~experimental_async_interface() {}
      // train - train
      virtual void train(::grpc::ClientContext* context, const ::tradingcore2pb::TrainData* request, ::tradingcore2pb::TrainResult* response, std::function<void(::grpc::Status)>) = 0;
      virtual void train(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingcore2pb::TrainResult* response, std::function<void(::grpc::Status)>) = 0;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void train(::grpc::ClientContext* context, const ::tradingcore2pb::TrainData* request, ::tradingcore2pb::TrainResult* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void train(::grpc::ClientContext* context, const ::tradingcore2pb::TrainData* request, ::tradingcore2pb::TrainResult* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void train(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingcore2pb::TrainResult* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void train(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingcore2pb::TrainResult* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
    };
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    typedef class experimental_async_interface async_interface;
    #endif
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    async_interface* async() { return experimental_async(); }
    #endif
    virtual class experimental_async_interface* experimental_async() { return nullptr; }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tradingcore2pb::TrainResult>* AsynctrainRaw(::grpc::ClientContext* context, const ::tradingcore2pb::TrainData& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tradingcore2pb::TrainResult>* PrepareAsynctrainRaw(::grpc::ClientContext* context, const ::tradingcore2pb::TrainData& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status train(::grpc::ClientContext* context, const ::tradingcore2pb::TrainData& request, ::tradingcore2pb::TrainResult* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tradingcore2pb::TrainResult>> Asynctrain(::grpc::ClientContext* context, const ::tradingcore2pb::TrainData& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tradingcore2pb::TrainResult>>(AsynctrainRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tradingcore2pb::TrainResult>> PrepareAsynctrain(::grpc::ClientContext* context, const ::tradingcore2pb::TrainData& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tradingcore2pb::TrainResult>>(PrepareAsynctrainRaw(context, request, cq));
    }
    class experimental_async final :
      public StubInterface::experimental_async_interface {
     public:
      void train(::grpc::ClientContext* context, const ::tradingcore2pb::TrainData* request, ::tradingcore2pb::TrainResult* response, std::function<void(::grpc::Status)>) override;
      void train(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingcore2pb::TrainResult* response, std::function<void(::grpc::Status)>) override;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void train(::grpc::ClientContext* context, const ::tradingcore2pb::TrainData* request, ::tradingcore2pb::TrainResult* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void train(::grpc::ClientContext* context, const ::tradingcore2pb::TrainData* request, ::tradingcore2pb::TrainResult* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void train(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingcore2pb::TrainResult* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void train(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingcore2pb::TrainResult* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
     private:
      friend class Stub;
      explicit experimental_async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class experimental_async_interface* experimental_async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class experimental_async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::tradingcore2pb::TrainResult>* AsynctrainRaw(::grpc::ClientContext* context, const ::tradingcore2pb::TrainData& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::tradingcore2pb::TrainResult>* PrepareAsynctrainRaw(::grpc::ClientContext* context, const ::tradingcore2pb::TrainData& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_train_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    // train - train
    virtual ::grpc::Status train(::grpc::ServerContext* context, const ::tradingcore2pb::TrainData* request, ::tradingcore2pb::TrainResult* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_train : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_train() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_train() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status train(::grpc::ServerContext* /*context*/, const ::tradingcore2pb::TrainData* /*request*/, ::tradingcore2pb::TrainResult* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void Requesttrain(::grpc::ServerContext* context, ::tradingcore2pb::TrainData* request, ::grpc::ServerAsyncResponseWriter< ::tradingcore2pb::TrainResult>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_train<Service > AsyncService;
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_train : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_train() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodCallback(0,
          new ::grpc_impl::internal::CallbackUnaryHandler< ::tradingcore2pb::TrainData, ::tradingcore2pb::TrainResult>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::tradingcore2pb::TrainData* request, ::tradingcore2pb::TrainResult* response) { return this->train(context, request, response); }));}
    void SetMessageAllocatorFor_train(
        ::grpc::experimental::MessageAllocator< ::tradingcore2pb::TrainData, ::tradingcore2pb::TrainResult>* allocator) {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
    #else
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::experimental().GetHandler(0);
    #endif
      static_cast<::grpc_impl::internal::CallbackUnaryHandler< ::tradingcore2pb::TrainData, ::tradingcore2pb::TrainResult>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~ExperimentalWithCallbackMethod_train() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status train(::grpc::ServerContext* /*context*/, const ::tradingcore2pb::TrainData* /*request*/, ::tradingcore2pb::TrainResult* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* train(
      ::grpc::CallbackServerContext* /*context*/, const ::tradingcore2pb::TrainData* /*request*/, ::tradingcore2pb::TrainResult* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* train(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::tradingcore2pb::TrainData* /*request*/, ::tradingcore2pb::TrainResult* /*response*/)
    #endif
      { return nullptr; }
  };
  #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
  typedef ExperimentalWithCallbackMethod_train<Service > CallbackService;
  #endif

  typedef ExperimentalWithCallbackMethod_train<Service > ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_train : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_train() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_train() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status train(::grpc::ServerContext* /*context*/, const ::tradingcore2pb::TrainData* /*request*/, ::tradingcore2pb::TrainResult* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_train : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_train() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_train() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status train(::grpc::ServerContext* /*context*/, const ::tradingcore2pb::TrainData* /*request*/, ::tradingcore2pb::TrainResult* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void Requesttrain(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_train : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_train() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodRawCallback(0,
          new ::grpc_impl::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->train(context, request, response); }));
    }
    ~ExperimentalWithRawCallbackMethod_train() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status train(::grpc::ServerContext* /*context*/, const ::tradingcore2pb::TrainData* /*request*/, ::tradingcore2pb::TrainResult* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* train(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* train(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_train : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_train() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::tradingcore2pb::TrainData, ::tradingcore2pb::TrainResult>(
            [this](::grpc_impl::ServerContext* context,
                   ::grpc_impl::ServerUnaryStreamer<
                     ::tradingcore2pb::TrainData, ::tradingcore2pb::TrainResult>* streamer) {
                       return this->Streamedtrain(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_train() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status train(::grpc::ServerContext* /*context*/, const ::tradingcore2pb::TrainData* /*request*/, ::tradingcore2pb::TrainResult* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status Streamedtrain(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::tradingcore2pb::TrainData,::tradingcore2pb::TrainResult>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_train<Service > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_train<Service > StreamedService;
};

}  // namespace tradingcore2pb


#endif  // GRPC_tradingcore2_2eproto__INCLUDED
