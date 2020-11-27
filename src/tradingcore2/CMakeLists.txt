include(CMakeToolsHelpers OPTIONAL)

cmake_minimum_required(VERSION 3.0.0)
project(libtradingcore2 VERSION 0.1.0)

file(STRINGS ../../VERSION TC2_VERSION)

configure_file(
    ../../include/tradingcore2/version.h.in
    version.h
    @ONLY)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(../../include)
include_directories(../../deps)
include_directories(../../deps/grpc/include)
include_directories(../../deps/grpc/third_party/protobuf/src)
include_directories(../../deps/rocksdb/include)
include_directories(../../deps/yaml-cpp/include)
include_directories(${gflags_INCLUDE_DIRS})
include_directories(../../deps/glog/cmakebuild)
include_directories(../../deps/glog/src)

aux_source_directory(. DIR_LIB_SRCS)
aux_source_directory(../../include/tradingcore2/protos DIR_LIB_PROTO_SRCS)
aux_source_directory(./exchange DIR_LIB_EXCHANGE_SRCS)
aux_source_directory(./indicator DIR_LIB_INDICATOR_SRCS)
aux_source_directory(./strategy DIR_LIB_STRATEGY_SRCS)
aux_source_directory(./server DIR_LIB_SERVER_SRCS)
aux_source_directory(./client DIR_LIB_CLIENT_SRCS)
aux_source_directory(./trdb2 DIR_LIB_TRDB2_SRCS)
aux_source_directory(./trnode2 DIR_LIB_TRNODE2_SRCS)
aux_source_directory(./ctrlcondition DIR_LIB_CTRLCONDITION_SRCS)

add_library(libtradingcore2 
    ${DIR_LIB_SRCS} 
    ${DIR_LIB_EXCHANGE_SRCS} 
    ${DIR_LIB_INDICATOR_SRCS} 
    ${DIR_LIB_STRATEGY_SRCS}
    ${DIR_LIB_PROTO_SRCS}
    ${DIR_LIB_SERVER_SRCS}
    ${DIR_LIB_CLIENT_SRCS}
    ${DIR_LIB_TRDB2_SRCS}
    ${DIR_LIB_TRNODE2_SRCS}
    ${DIR_LIB_CTRLCONDITION_SRCS})