protoc -I proto --grpc_out=src/tradingcore2/proto --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` proto/tradingcore2.proto
protoc -I proto --cpp_out=src/tradingcore2/proto proto/tradingcore2.proto
protoc -I proto --grpc_out=src/tradingcore2/proto --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` proto/tradingdb2.proto
protoc -I proto --cpp_out=src/tradingcore2/proto proto/tradingdb2.proto