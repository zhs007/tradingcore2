include(CMakeToolsHelpers OPTIONAL)

cmake_minimum_required(VERSION 3.0.0)
project(libtradingcore2 VERSION 0.1.0)

include_directories(../../include)
include_directories(../../deps)
include_directories(../../deps/grpc/include)
include_directories(../../deps/grpc/third_party/protobuf/src)
include_directories(../../deps/rocksdb/include)

aux_source_directory(. DIR_LIB_SRCS)
aux_source_directory(./exchange DIR_LIB_EXCHANGE_SRCS)
aux_source_directory(./indicator DIR_LIB_INDICATOR_SRCS)
aux_source_directory(./strategy DIR_LIB_STRATEGY_SRCS)
aux_source_directory(./proto DIR_LIB_PROTO_SRCS)
aux_source_directory(./server DIR_LIB_SERVER_SRCS)

add_library(libtradingcore2 
    ${DIR_LIB_SRCS} 
    ${DIR_LIB_EXCHANGE_SRCS} 
    ${DIR_LIB_INDICATOR_SRCS} 
    ${DIR_LIB_STRATEGY_SRCS}
    ${DIR_LIB_PROTO_SRCS}
    ${DIR_LIB_SERVER_SRCS})