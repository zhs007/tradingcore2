cmake_minimum_required(VERSION 3.0.0)

include_directories(../../include)
# include_directories(${CMAKE_BINARY_DIR}/deps/glog)

link_directories(../../deps/gsl/build)
link_directories(../../deps/yaml-cpp/build)
link_directories(../../deps/glog/cmakebuild)

aux_source_directory(. DIR_MAIN_SRCS)

add_executable(tc2serv ${DIR_MAIN_SRCS})

target_link_libraries(tc2serv libtradingcore2)
target_link_libraries(tc2serv libgsl.a)
target_link_libraries(tc2serv grpc)
target_link_libraries(tc2serv grpc++)
target_link_libraries(tc2serv grpc++_reflection)
target_link_libraries(tc2serv librocksdb.a)
target_link_libraries(tc2serv libyaml-cpp.a)
target_link_libraries(tc2serv libglog.a)
target_link_libraries(tc2serv libgflags.a)