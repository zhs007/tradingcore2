// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: tradingdb2.proto

#include "tradingdb2.pb.h"
#include "tradingdb2.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace tradingpb {

static const char* TradingDB2_method_names[] = {
  "/tradingpb.TradingDB2/updCandles",
  "/tradingpb.TradingDB2/getCandles",
  "/tradingpb.TradingDB2/updSymbol",
  "/tradingpb.TradingDB2/getSymbol",
  "/tradingpb.TradingDB2/getSymbols",
  "/tradingpb.TradingDB2/simTrading",
};

std::unique_ptr< TradingDB2::Stub> TradingDB2::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< TradingDB2::Stub> stub(new TradingDB2::Stub(channel));
  return stub;
}

TradingDB2::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_updCandles_(TradingDB2_method_names[0], ::grpc::internal::RpcMethod::CLIENT_STREAMING, channel)
  , rpcmethod_getCandles_(TradingDB2_method_names[1], ::grpc::internal::RpcMethod::SERVER_STREAMING, channel)
  , rpcmethod_updSymbol_(TradingDB2_method_names[2], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_getSymbol_(TradingDB2_method_names[3], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_getSymbols_(TradingDB2_method_names[4], ::grpc::internal::RpcMethod::SERVER_STREAMING, channel)
  , rpcmethod_simTrading_(TradingDB2_method_names[5], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::ClientWriter< ::tradingpb::RequestUpdCandles>* TradingDB2::Stub::updCandlesRaw(::grpc::ClientContext* context, ::tradingpb::ReplyUpdCandles* response) {
  return ::grpc_impl::internal::ClientWriterFactory< ::tradingpb::RequestUpdCandles>::Create(channel_.get(), rpcmethod_updCandles_, context, response);
}

void TradingDB2::Stub::experimental_async::updCandles(::grpc::ClientContext* context, ::tradingpb::ReplyUpdCandles* response, ::grpc::experimental::ClientWriteReactor< ::tradingpb::RequestUpdCandles>* reactor) {
  ::grpc_impl::internal::ClientCallbackWriterFactory< ::tradingpb::RequestUpdCandles>::Create(stub_->channel_.get(), stub_->rpcmethod_updCandles_, context, response, reactor);
}

::grpc::ClientAsyncWriter< ::tradingpb::RequestUpdCandles>* TradingDB2::Stub::AsyncupdCandlesRaw(::grpc::ClientContext* context, ::tradingpb::ReplyUpdCandles* response, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc_impl::internal::ClientAsyncWriterFactory< ::tradingpb::RequestUpdCandles>::Create(channel_.get(), cq, rpcmethod_updCandles_, context, response, true, tag);
}

::grpc::ClientAsyncWriter< ::tradingpb::RequestUpdCandles>* TradingDB2::Stub::PrepareAsyncupdCandlesRaw(::grpc::ClientContext* context, ::tradingpb::ReplyUpdCandles* response, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncWriterFactory< ::tradingpb::RequestUpdCandles>::Create(channel_.get(), cq, rpcmethod_updCandles_, context, response, false, nullptr);
}

::grpc::ClientReader< ::tradingpb::ReplyGetCandles>* TradingDB2::Stub::getCandlesRaw(::grpc::ClientContext* context, const ::tradingpb::RequestGetCandles& request) {
  return ::grpc_impl::internal::ClientReaderFactory< ::tradingpb::ReplyGetCandles>::Create(channel_.get(), rpcmethod_getCandles_, context, request);
}

void TradingDB2::Stub::experimental_async::getCandles(::grpc::ClientContext* context, ::tradingpb::RequestGetCandles* request, ::grpc::experimental::ClientReadReactor< ::tradingpb::ReplyGetCandles>* reactor) {
  ::grpc_impl::internal::ClientCallbackReaderFactory< ::tradingpb::ReplyGetCandles>::Create(stub_->channel_.get(), stub_->rpcmethod_getCandles_, context, request, reactor);
}

::grpc::ClientAsyncReader< ::tradingpb::ReplyGetCandles>* TradingDB2::Stub::AsyncgetCandlesRaw(::grpc::ClientContext* context, const ::tradingpb::RequestGetCandles& request, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc_impl::internal::ClientAsyncReaderFactory< ::tradingpb::ReplyGetCandles>::Create(channel_.get(), cq, rpcmethod_getCandles_, context, request, true, tag);
}

::grpc::ClientAsyncReader< ::tradingpb::ReplyGetCandles>* TradingDB2::Stub::PrepareAsyncgetCandlesRaw(::grpc::ClientContext* context, const ::tradingpb::RequestGetCandles& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncReaderFactory< ::tradingpb::ReplyGetCandles>::Create(channel_.get(), cq, rpcmethod_getCandles_, context, request, false, nullptr);
}

::grpc::Status TradingDB2::Stub::updSymbol(::grpc::ClientContext* context, const ::tradingpb::RequestUpdSymbol& request, ::tradingpb::ReplyUpdSymbol* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_updSymbol_, context, request, response);
}

void TradingDB2::Stub::experimental_async::updSymbol(::grpc::ClientContext* context, const ::tradingpb::RequestUpdSymbol* request, ::tradingpb::ReplyUpdSymbol* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_updSymbol_, context, request, response, std::move(f));
}

void TradingDB2::Stub::experimental_async::updSymbol(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingpb::ReplyUpdSymbol* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_updSymbol_, context, request, response, std::move(f));
}

void TradingDB2::Stub::experimental_async::updSymbol(::grpc::ClientContext* context, const ::tradingpb::RequestUpdSymbol* request, ::tradingpb::ReplyUpdSymbol* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_updSymbol_, context, request, response, reactor);
}

void TradingDB2::Stub::experimental_async::updSymbol(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingpb::ReplyUpdSymbol* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_updSymbol_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::tradingpb::ReplyUpdSymbol>* TradingDB2::Stub::AsyncupdSymbolRaw(::grpc::ClientContext* context, const ::tradingpb::RequestUpdSymbol& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::tradingpb::ReplyUpdSymbol>::Create(channel_.get(), cq, rpcmethod_updSymbol_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::tradingpb::ReplyUpdSymbol>* TradingDB2::Stub::PrepareAsyncupdSymbolRaw(::grpc::ClientContext* context, const ::tradingpb::RequestUpdSymbol& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::tradingpb::ReplyUpdSymbol>::Create(channel_.get(), cq, rpcmethod_updSymbol_, context, request, false);
}

::grpc::Status TradingDB2::Stub::getSymbol(::grpc::ClientContext* context, const ::tradingpb::RequestGetSymbol& request, ::tradingpb::ReplyGetSymbol* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_getSymbol_, context, request, response);
}

void TradingDB2::Stub::experimental_async::getSymbol(::grpc::ClientContext* context, const ::tradingpb::RequestGetSymbol* request, ::tradingpb::ReplyGetSymbol* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_getSymbol_, context, request, response, std::move(f));
}

void TradingDB2::Stub::experimental_async::getSymbol(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingpb::ReplyGetSymbol* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_getSymbol_, context, request, response, std::move(f));
}

void TradingDB2::Stub::experimental_async::getSymbol(::grpc::ClientContext* context, const ::tradingpb::RequestGetSymbol* request, ::tradingpb::ReplyGetSymbol* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_getSymbol_, context, request, response, reactor);
}

void TradingDB2::Stub::experimental_async::getSymbol(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingpb::ReplyGetSymbol* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_getSymbol_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::tradingpb::ReplyGetSymbol>* TradingDB2::Stub::AsyncgetSymbolRaw(::grpc::ClientContext* context, const ::tradingpb::RequestGetSymbol& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::tradingpb::ReplyGetSymbol>::Create(channel_.get(), cq, rpcmethod_getSymbol_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::tradingpb::ReplyGetSymbol>* TradingDB2::Stub::PrepareAsyncgetSymbolRaw(::grpc::ClientContext* context, const ::tradingpb::RequestGetSymbol& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::tradingpb::ReplyGetSymbol>::Create(channel_.get(), cq, rpcmethod_getSymbol_, context, request, false);
}

::grpc::ClientReader< ::tradingpb::ReplyGetSymbol>* TradingDB2::Stub::getSymbolsRaw(::grpc::ClientContext* context, const ::tradingpb::RequestGetSymbols& request) {
  return ::grpc_impl::internal::ClientReaderFactory< ::tradingpb::ReplyGetSymbol>::Create(channel_.get(), rpcmethod_getSymbols_, context, request);
}

void TradingDB2::Stub::experimental_async::getSymbols(::grpc::ClientContext* context, ::tradingpb::RequestGetSymbols* request, ::grpc::experimental::ClientReadReactor< ::tradingpb::ReplyGetSymbol>* reactor) {
  ::grpc_impl::internal::ClientCallbackReaderFactory< ::tradingpb::ReplyGetSymbol>::Create(stub_->channel_.get(), stub_->rpcmethod_getSymbols_, context, request, reactor);
}

::grpc::ClientAsyncReader< ::tradingpb::ReplyGetSymbol>* TradingDB2::Stub::AsyncgetSymbolsRaw(::grpc::ClientContext* context, const ::tradingpb::RequestGetSymbols& request, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc_impl::internal::ClientAsyncReaderFactory< ::tradingpb::ReplyGetSymbol>::Create(channel_.get(), cq, rpcmethod_getSymbols_, context, request, true, tag);
}

::grpc::ClientAsyncReader< ::tradingpb::ReplyGetSymbol>* TradingDB2::Stub::PrepareAsyncgetSymbolsRaw(::grpc::ClientContext* context, const ::tradingpb::RequestGetSymbols& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncReaderFactory< ::tradingpb::ReplyGetSymbol>::Create(channel_.get(), cq, rpcmethod_getSymbols_, context, request, false, nullptr);
}

::grpc::Status TradingDB2::Stub::simTrading(::grpc::ClientContext* context, const ::tradingpb::RequestSimTrading& request, ::tradingpb::ReplySimTrading* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_simTrading_, context, request, response);
}

void TradingDB2::Stub::experimental_async::simTrading(::grpc::ClientContext* context, const ::tradingpb::RequestSimTrading* request, ::tradingpb::ReplySimTrading* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_simTrading_, context, request, response, std::move(f));
}

void TradingDB2::Stub::experimental_async::simTrading(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingpb::ReplySimTrading* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_simTrading_, context, request, response, std::move(f));
}

void TradingDB2::Stub::experimental_async::simTrading(::grpc::ClientContext* context, const ::tradingpb::RequestSimTrading* request, ::tradingpb::ReplySimTrading* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_simTrading_, context, request, response, reactor);
}

void TradingDB2::Stub::experimental_async::simTrading(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::tradingpb::ReplySimTrading* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_simTrading_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::tradingpb::ReplySimTrading>* TradingDB2::Stub::AsyncsimTradingRaw(::grpc::ClientContext* context, const ::tradingpb::RequestSimTrading& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::tradingpb::ReplySimTrading>::Create(channel_.get(), cq, rpcmethod_simTrading_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::tradingpb::ReplySimTrading>* TradingDB2::Stub::PrepareAsyncsimTradingRaw(::grpc::ClientContext* context, const ::tradingpb::RequestSimTrading& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::tradingpb::ReplySimTrading>::Create(channel_.get(), cq, rpcmethod_simTrading_, context, request, false);
}

TradingDB2::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      TradingDB2_method_names[0],
      ::grpc::internal::RpcMethod::CLIENT_STREAMING,
      new ::grpc::internal::ClientStreamingHandler< TradingDB2::Service, ::tradingpb::RequestUpdCandles, ::tradingpb::ReplyUpdCandles>(
          [](TradingDB2::Service* service,
             ::grpc_impl::ServerContext* ctx,
             ::grpc_impl::ServerReader<::tradingpb::RequestUpdCandles>* reader,
             ::tradingpb::ReplyUpdCandles* resp) {
               return service->updCandles(ctx, reader, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      TradingDB2_method_names[1],
      ::grpc::internal::RpcMethod::SERVER_STREAMING,
      new ::grpc::internal::ServerStreamingHandler< TradingDB2::Service, ::tradingpb::RequestGetCandles, ::tradingpb::ReplyGetCandles>(
          [](TradingDB2::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::tradingpb::RequestGetCandles* req,
             ::grpc_impl::ServerWriter<::tradingpb::ReplyGetCandles>* writer) {
               return service->getCandles(ctx, req, writer);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      TradingDB2_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< TradingDB2::Service, ::tradingpb::RequestUpdSymbol, ::tradingpb::ReplyUpdSymbol>(
          [](TradingDB2::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::tradingpb::RequestUpdSymbol* req,
             ::tradingpb::ReplyUpdSymbol* resp) {
               return service->updSymbol(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      TradingDB2_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< TradingDB2::Service, ::tradingpb::RequestGetSymbol, ::tradingpb::ReplyGetSymbol>(
          [](TradingDB2::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::tradingpb::RequestGetSymbol* req,
             ::tradingpb::ReplyGetSymbol* resp) {
               return service->getSymbol(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      TradingDB2_method_names[4],
      ::grpc::internal::RpcMethod::SERVER_STREAMING,
      new ::grpc::internal::ServerStreamingHandler< TradingDB2::Service, ::tradingpb::RequestGetSymbols, ::tradingpb::ReplyGetSymbol>(
          [](TradingDB2::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::tradingpb::RequestGetSymbols* req,
             ::grpc_impl::ServerWriter<::tradingpb::ReplyGetSymbol>* writer) {
               return service->getSymbols(ctx, req, writer);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      TradingDB2_method_names[5],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< TradingDB2::Service, ::tradingpb::RequestSimTrading, ::tradingpb::ReplySimTrading>(
          [](TradingDB2::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::tradingpb::RequestSimTrading* req,
             ::tradingpb::ReplySimTrading* resp) {
               return service->simTrading(ctx, req, resp);
             }, this)));
}

TradingDB2::Service::~Service() {
}

::grpc::Status TradingDB2::Service::updCandles(::grpc::ServerContext* context, ::grpc::ServerReader< ::tradingpb::RequestUpdCandles>* reader, ::tradingpb::ReplyUpdCandles* response) {
  (void) context;
  (void) reader;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status TradingDB2::Service::getCandles(::grpc::ServerContext* context, const ::tradingpb::RequestGetCandles* request, ::grpc::ServerWriter< ::tradingpb::ReplyGetCandles>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status TradingDB2::Service::updSymbol(::grpc::ServerContext* context, const ::tradingpb::RequestUpdSymbol* request, ::tradingpb::ReplyUpdSymbol* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status TradingDB2::Service::getSymbol(::grpc::ServerContext* context, const ::tradingpb::RequestGetSymbol* request, ::tradingpb::ReplyGetSymbol* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status TradingDB2::Service::getSymbols(::grpc::ServerContext* context, const ::tradingpb::RequestGetSymbols* request, ::grpc::ServerWriter< ::tradingpb::ReplyGetSymbol>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status TradingDB2::Service::simTrading(::grpc::ServerContext* context, const ::tradingpb::RequestSimTrading* request, ::tradingpb::ReplySimTrading* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace tradingpb

