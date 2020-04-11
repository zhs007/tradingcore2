cmake_minimum_required(VERSION 3.0.0)

include_directories(../../include)
# include_directories(${CMAKE_BINARY_DIR}/deps/glog)

link_directories(../../deps/gsl/build)
link_directories(../../deps/yaml-cpp/build)
link_directories(../../deps/glog/cmakebuild)

aux_source_directory(. DIR_MAIN_SRCS)

add_executable(client ${DIR_MAIN_SRCS})

target_link_libraries(client libtradingcore2)
target_link_libraries(client libgsl.a)
target_link_libraries(client grpc)
target_link_libraries(client grpc++)
target_link_libraries(client grpc++_reflection)
target_link_libraries(client librocksdb.a)
target_link_libraries(client libyaml-cpp.a)
target_link_libraries(client libglog.a)