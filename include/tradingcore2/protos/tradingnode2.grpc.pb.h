// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: tradingnode2.proto
#ifndef GRPC_tradingnode2_2eproto__INCLUDED
#define GRPC_tradingnode2_2eproto__INCLUDED

#include "tradingnode2.pb.h"

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

namespace tradingpb {

// TradingNode2 - TradingNode2 service
class TradingNode2 final {
 public:
  static constexpr char const* service_full_name() {
    return "tradingpb.TradingNode2";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    // getServerInfo - get server infomation
    virtual ::grpc::Status getServerInfo(::grpc::ClientContext* context, const ::tradingpb::RequestServerInfo& request, ::tradingpb::ReplyServerInfo* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tradingpb::ReplyServerInfo>> AsyncgetServerInfo(::grpc::ClientContext* context, const ::tradingpb::RequestServerInfo& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tradingpb::ReplyServerInfo>>(AsyncgetServerInfoRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tradingpb::ReplyServerInfo>> PrepareAsyncgetServerInfo(::grpc::ClientContext* context, const ::tradingpb::RequestServerInfo& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tradingpb::ReplyServerInfo>>(PrepareAsyncgetServerInfoRaw(context, request, cq));
    }
    // calcPNL - calc PNL
    virtual ::grpc::Status calcPNL(::grpc::ClientContext* context, const ::tradingpb::RequestCalcPNL& request, ::tradingpb::ReplyCalcPNL* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tradingpb::ReplyCalcPNL>> AsynccalcPNL(::grpc::ClientContext* context, const ::tradingpb::RequestCalcPNL& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tradingpb::ReplyCalcPNL>>(AsynccalcPNLRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tradingpb::ReplyCalcPNL>> PrepareAsynccalcPNL(::grpc::ClientContext* context, const ::tradingpb::RequestCalcPNL& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tradingpb::ReplyCalcPNL>>(PrepareAsynccalcPNLRaw(context, request, cq));
    }
    class experimental_async_interface {
     public:
      virtual ~experimental_async_interface() {}
      // getServerInfo - get server infomation
      virtual void getServerInfo(::grpc::ClientContext* context, const ::tradingpb::RequestServerInfo* request, ::tradingpb::ReplyServerInfo* response, std::function<void(::grpc::Status)>) = 0;
      virtual void getServerInfo(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingpb::ReplyServerInfo* response, std::function<void(::grpc::Status)>) = 0;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void getServerInfo(::grpc::ClientContext* context, const ::tradingpb::RequestServerInfo* request, ::tradingpb::ReplyServerInfo* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void getServerInfo(::grpc::ClientContext* context, const ::tradingpb::RequestServerInfo* request, ::tradingpb::ReplyServerInfo* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void getServerInfo(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingpb::ReplyServerInfo* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void getServerInfo(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingpb::ReplyServerInfo* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
      // calcPNL - calc PNL
      virtual void calcPNL(::grpc::ClientContext* context, const ::tradingpb::RequestCalcPNL* request, ::tradingpb::ReplyCalcPNL* response, std::function<void(::grpc::Status)>) = 0;
      virtual void calcPNL(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingpb::ReplyCalcPNL* response, std::function<void(::grpc::Status)>) = 0;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void calcPNL(::grpc::ClientContext* context, const ::tradingpb::RequestCalcPNL* request, ::tradingpb::ReplyCalcPNL* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void calcPNL(::grpc::ClientContext* context, const ::tradingpb::RequestCalcPNL* request, ::tradingpb::ReplyCalcPNL* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void calcPNL(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingpb::ReplyCalcPNL* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void calcPNL(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingpb::ReplyCalcPNL* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
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
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tradingpb::ReplyServerInfo>* AsyncgetServerInfoRaw(::grpc::ClientContext* context, const ::tradingpb::RequestServerInfo& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tradingpb::ReplyServerInfo>* PrepareAsyncgetServerInfoRaw(::grpc::ClientContext* context, const ::tradingpb::RequestServerInfo& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tradingpb::ReplyCalcPNL>* AsynccalcPNLRaw(::grpc::ClientContext* context, const ::tradingpb::RequestCalcPNL& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tradingpb::ReplyCalcPNL>* PrepareAsynccalcPNLRaw(::grpc::ClientContext* context, const ::tradingpb::RequestCalcPNL& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status getServerInfo(::grpc::ClientContext* context, const ::tradingpb::RequestServerInfo& request, ::tradingpb::ReplyServerInfo* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tradingpb::ReplyServerInfo>> AsyncgetServerInfo(::grpc::ClientContext* context, const ::tradingpb::RequestServerInfo& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tradingpb::ReplyServerInfo>>(AsyncgetServerInfoRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tradingpb::ReplyServerInfo>> PrepareAsyncgetServerInfo(::grpc::ClientContext* context, const ::tradingpb::RequestServerInfo& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tradingpb::ReplyServerInfo>>(PrepareAsyncgetServerInfoRaw(context, request, cq));
    }
    ::grpc::Status calcPNL(::grpc::ClientContext* context, const ::tradingpb::RequestCalcPNL& request, ::tradingpb::ReplyCalcPNL* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tradingpb::ReplyCalcPNL>> AsynccalcPNL(::grpc::ClientContext* context, const ::tradingpb::RequestCalcPNL& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tradingpb::ReplyCalcPNL>>(AsynccalcPNLRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tradingpb::ReplyCalcPNL>> PrepareAsynccalcPNL(::grpc::ClientContext* context, const ::tradingpb::RequestCalcPNL& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tradingpb::ReplyCalcPNL>>(PrepareAsynccalcPNLRaw(context, request, cq));
    }
    class experimental_async final :
      public StubInterface::experimental_async_interface {
     public:
      void getServerInfo(::grpc::ClientContext* context, const ::tradingpb::RequestServerInfo* request, ::tradingpb::ReplyServerInfo* response, std::function<void(::grpc::Status)>) override;
      void getServerInfo(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingpb::ReplyServerInfo* response, std::function<void(::grpc::Status)>) override;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void getServerInfo(::grpc::ClientContext* context, const ::tradingpb::RequestServerInfo* request, ::tradingpb::ReplyServerInfo* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void getServerInfo(::grpc::ClientContext* context, const ::tradingpb::RequestServerInfo* request, ::tradingpb::ReplyServerInfo* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void getServerInfo(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingpb::ReplyServerInfo* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void getServerInfo(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingpb::ReplyServerInfo* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
      void calcPNL(::grpc::ClientContext* context, const ::tradingpb::RequestCalcPNL* request, ::tradingpb::ReplyCalcPNL* response, std::function<void(::grpc::Status)>) override;
      void calcPNL(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingpb::ReplyCalcPNL* response, std::function<void(::grpc::Status)>) override;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void calcPNL(::grpc::ClientContext* context, const ::tradingpb::RequestCalcPNL* request, ::tradingpb::ReplyCalcPNL* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void calcPNL(::grpc::ClientContext* context, const ::tradingpb::RequestCalcPNL* request, ::tradingpb::ReplyCalcPNL* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void calcPNL(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingpb::ReplyCalcPNL* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void calcPNL(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingpb::ReplyCalcPNL* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
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
    ::grpc::ClientAsyncResponseReader< ::tradingpb::ReplyServerInfo>* AsyncgetServerInfoRaw(::grpc::ClientContext* context, const ::tradingpb::RequestServerInfo& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::tradingpb::ReplyServerInfo>* PrepareAsyncgetServerInfoRaw(::grpc::ClientContext* context, const ::tradingpb::RequestServerInfo& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::tradingpb::ReplyCalcPNL>* AsynccalcPNLRaw(::grpc::ClientContext* context, const ::tradingpb::RequestCalcPNL& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::tradingpb::ReplyCalcPNL>* PrepareAsynccalcPNLRaw(::grpc::ClientContext* context, const ::tradingpb::RequestCalcPNL& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_getServerInfo_;
    const ::grpc::internal::RpcMethod rpcmethod_calcPNL_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    // getServerInfo - get server infomation
    virtual ::grpc::Status getServerInfo(::grpc::ServerContext* context, const ::tradingpb::RequestServerInfo* request, ::tradingpb::ReplyServerInfo* response);
    // calcPNL - calc PNL
    virtual ::grpc::Status calcPNL(::grpc::ServerContext* context, const ::tradingpb::RequestCalcPNL* request, ::tradingpb::ReplyCalcPNL* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_getServerInfo : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_getServerInfo() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_getServerInfo() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status getServerInfo(::grpc::ServerContext* /*context*/, const ::tradingpb::RequestServerInfo* /*request*/, ::tradingpb::ReplyServerInfo* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestgetServerInfo(::grpc::ServerContext* context, ::tradingpb::RequestServerInfo* request, ::grpc::ServerAsyncResponseWriter< ::tradingpb::ReplyServerInfo>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_calcPNL : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_calcPNL() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_calcPNL() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status calcPNL(::grpc::ServerContext* /*context*/, const ::tradingpb::RequestCalcPNL* /*request*/, ::tradingpb::ReplyCalcPNL* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestcalcPNL(::grpc::ServerContext* context, ::tradingpb::RequestCalcPNL* request, ::grpc::ServerAsyncResponseWriter< ::tradingpb::ReplyCalcPNL>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_getServerInfo<WithAsyncMethod_calcPNL<Service > > AsyncService;
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_getServerInfo : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_getServerInfo() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodCallback(0,
          new ::grpc_impl::internal::CallbackUnaryHandler< ::tradingpb::RequestServerInfo, ::tradingpb::ReplyServerInfo>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::tradingpb::RequestServerInfo* request, ::tradingpb::ReplyServerInfo* response) { return this->getServerInfo(context, request, response); }));}
    void SetMessageAllocatorFor_getServerInfo(
        ::grpc::experimental::MessageAllocator< ::tradingpb::RequestServerInfo, ::tradingpb::ReplyServerInfo>* allocator) {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
    #else
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::experimental().GetHandler(0);
    #endif
      static_cast<::grpc_impl::internal::CallbackUnaryHandler< ::tradingpb::RequestServerInfo, ::tradingpb::ReplyServerInfo>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~ExperimentalWithCallbackMethod_getServerInfo() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status getServerInfo(::grpc::ServerContext* /*context*/, const ::tradingpb::RequestServerInfo* /*request*/, ::tradingpb::ReplyServerInfo* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* getServerInfo(
      ::grpc::CallbackServerContext* /*context*/, const ::tradingpb::RequestServerInfo* /*request*/, ::tradingpb::ReplyServerInfo* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* getServerInfo(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::tradingpb::RequestServerInfo* /*request*/, ::tradingpb::ReplyServerInfo* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_calcPNL : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_calcPNL() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodCallback(1,
          new ::grpc_impl::internal::CallbackUnaryHandler< ::tradingpb::RequestCalcPNL, ::tradingpb::ReplyCalcPNL>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::tradingpb::RequestCalcPNL* request, ::tradingpb::ReplyCalcPNL* response) { return this->calcPNL(context, request, response); }));}
    void SetMessageAllocatorFor_calcPNL(
        ::grpc::experimental::MessageAllocator< ::tradingpb::RequestCalcPNL, ::tradingpb::ReplyCalcPNL>* allocator) {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(1);
    #else
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::experimental().GetHandler(1);
    #endif
      static_cast<::grpc_impl::internal::CallbackUnaryHandler< ::tradingpb::RequestCalcPNL, ::tradingpb::ReplyCalcPNL>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~ExperimentalWithCallbackMethod_calcPNL() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status calcPNL(::grpc::ServerContext* /*context*/, const ::tradingpb::RequestCalcPNL* /*request*/, ::tradingpb::ReplyCalcPNL* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* calcPNL(
      ::grpc::CallbackServerContext* /*context*/, const ::tradingpb::RequestCalcPNL* /*request*/, ::tradingpb::ReplyCalcPNL* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* calcPNL(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::tradingpb::RequestCalcPNL* /*request*/, ::tradingpb::ReplyCalcPNL* /*response*/)
    #endif
      { return nullptr; }
  };
  #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
  typedef ExperimentalWithCallbackMethod_getServerInfo<ExperimentalWithCallbackMethod_calcPNL<Service > > CallbackService;
  #endif

  typedef ExperimentalWithCallbackMethod_getServerInfo<ExperimentalWithCallbackMethod_calcPNL<Service > > ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_getServerInfo : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_getServerInfo() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_getServerInfo() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status getServerInfo(::grpc::ServerContext* /*context*/, const ::tradingpb::RequestServerInfo* /*request*/, ::tradingpb::ReplyServerInfo* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_calcPNL : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_calcPNL() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_calcPNL() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status calcPNL(::grpc::ServerContext* /*context*/, const ::tradingpb::RequestCalcPNL* /*request*/, ::tradingpb::ReplyCalcPNL* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_getServerInfo : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_getServerInfo() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_getServerInfo() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status getServerInfo(::grpc::ServerContext* /*context*/, const ::tradingpb::RequestServerInfo* /*request*/, ::tradingpb::ReplyServerInfo* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestgetServerInfo(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_calcPNL : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_calcPNL() {
      ::grpc::Service::MarkMethodRaw(1);
    }
    ~WithRawMethod_calcPNL() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status calcPNL(::grpc::ServerContext* /*context*/, const ::tradingpb::RequestCalcPNL* /*request*/, ::tradingpb::ReplyCalcPNL* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestcalcPNL(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_getServerInfo : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_getServerInfo() {
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
                     context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->getServerInfo(context, request, response); }));
    }
    ~ExperimentalWithRawCallbackMethod_getServerInfo() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status getServerInfo(::grpc::ServerContext* /*context*/, const ::tradingpb::RequestServerInfo* /*request*/, ::tradingpb::ReplyServerInfo* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* getServerInfo(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* getServerInfo(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_calcPNL : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_calcPNL() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodRawCallback(1,
          new ::grpc_impl::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->calcPNL(context, request, response); }));
    }
    ~ExperimentalWithRawCallbackMethod_calcPNL() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status calcPNL(::grpc::ServerContext* /*context*/, const ::tradingpb::RequestCalcPNL* /*request*/, ::tradingpb::ReplyCalcPNL* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* calcPNL(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* calcPNL(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_getServerInfo : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_getServerInfo() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::tradingpb::RequestServerInfo, ::tradingpb::ReplyServerInfo>(
            [this](::grpc_impl::ServerContext* context,
                   ::grpc_impl::ServerUnaryStreamer<
                     ::tradingpb::RequestServerInfo, ::tradingpb::ReplyServerInfo>* streamer) {
                       return this->StreamedgetServerInfo(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_getServerInfo() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status getServerInfo(::grpc::ServerContext* /*context*/, const ::tradingpb::RequestServerInfo* /*request*/, ::tradingpb::ReplyServerInfo* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedgetServerInfo(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::tradingpb::RequestServerInfo,::tradingpb::ReplyServerInfo>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_calcPNL : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_calcPNL() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::internal::StreamedUnaryHandler<
          ::tradingpb::RequestCalcPNL, ::tradingpb::ReplyCalcPNL>(
            [this](::grpc_impl::ServerContext* context,
                   ::grpc_impl::ServerUnaryStreamer<
                     ::tradingpb::RequestCalcPNL, ::tradingpb::ReplyCalcPNL>* streamer) {
                       return this->StreamedcalcPNL(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_calcPNL() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status calcPNL(::grpc::ServerContext* /*context*/, const ::tradingpb::RequestCalcPNL* /*request*/, ::tradingpb::ReplyCalcPNL* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedcalcPNL(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::tradingpb::RequestCalcPNL,::tradingpb::ReplyCalcPNL>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_getServerInfo<WithStreamedUnaryMethod_calcPNL<Service > > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_getServerInfo<WithStreamedUnaryMethod_calcPNL<Service > > StreamedService;
};

}  // namespace tradingpb


#endif  // GRPC_tradingnode2_2eproto__INCLUDED
