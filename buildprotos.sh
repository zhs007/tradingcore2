protoc -I protos --grpc_out=include/tradingcore2/protos --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` protos/tradingcore2.proto
protoc -I protos --cpp_out=include/tradingcore2/protos protos/tradingcore2.proto
# protoc -I protos --grpc_out=include/tradingcore2/protos --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` protos/trading2.proto
protoc -I protos --cpp_out=include/tradingcore2/protos protos/trading2.proto
protoc -I protos --grpc_out=include/tradingcore2/protos --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` protos/tradingdb2.proto
protoc -I protos --cpp_out=include/tradingcore2/protos protos/tradingdb2.proto
protoc -I protos --grpc_out=include/tradingcore2/protos --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` protos/tradingnode2.proto
protoc -I protos --cpp_out=include/tradingcore2/protos protos/tradingnode2.proto