cmake_minimum_required(VERSION 3.0.0)

include_directories(../../include)
# include_directories(${CMAKE_BINARY_DIR}/deps/glog)

link_directories(../../deps/gsl/build)
link_directories(../../deps/yaml-cpp/build)
link_directories(../../deps/glog/cmakebuild)

aux_source_directory(. DIR_MAIN_SRCS)

add_executable(trdb2client ${DIR_MAIN_SRCS})

target_link_libraries(trdb2client libtradingcore2)
target_link_libraries(trdb2client libgsl.a)
target_link_libraries(trdb2client grpc)
target_link_libraries(trdb2client grpc++)
target_link_libraries(trdb2client grpc++_reflection)
target_link_libraries(trdb2client librocksdb.a)
target_link_libraries(trdb2client libyaml-cpp.a)
target_link_libraries(trdb2client libglog.a)
target_link_libraries(trdb2client libgflags.a)