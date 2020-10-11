cmake_minimum_required(VERSION 3.0.0)

include_directories(../../include)
# include_directories(${CMAKE_BINARY_DIR}/deps/glog)

link_directories(../../deps/gsl/build)
link_directories(../../deps/yaml-cpp/build)
link_directories(../../deps/glog/cmakebuild)

aux_source_directory(. DIR_MAIN_SRCS)

add_executable(calcfund ${DIR_MAIN_SRCS})

target_link_libraries(calcfund libtradingcore2)
target_link_libraries(calcfund libgsl.a)
target_link_libraries(calcfund grpc)
target_link_libraries(calcfund grpc++)
target_link_libraries(calcfund grpc++_reflection)
target_link_libraries(calcfund librocksdb.a)
target_link_libraries(calcfund libyaml-cpp.a)
target_link_libraries(calcfund libglog.a)
target_link_libraries(calcfund libgflags.a)