cmake_minimum_required(VERSION 3.0.0)

include_directories(../../include)
# include_directories(${CMAKE_BINARY_DIR}/deps/glog)

link_directories(../../deps/gsl/build)
link_directories(../../deps/yaml-cpp/build)
link_directories(../../deps/glog/cmakebuild)

aux_source_directory(. DIR_MAIN_SRCS)

add_executable(tc2client ${DIR_MAIN_SRCS})

target_link_libraries(tc2client libtradingcore2)
target_link_libraries(tc2client libgsl.a)
target_link_libraries(tc2client grpc)
target_link_libraries(tc2client grpc++)
target_link_libraries(tc2client grpc++_reflection)
target_link_libraries(tc2client librocksdb.a)
target_link_libraries(tc2client libyaml-cpp.a)
target_link_libraries(tc2client libglog.a)
target_link_libraries(tc2client libgflags.a)