cmake_minimum_required(VERSION 3.0.0)

include_directories(../include)

link_directories(../../deps/gsl/build)
link_directories(../../deps/yaml-cpp/build)

aux_source_directory(. DIR_MAIN_SRCS)

add_executable(server ${DIR_MAIN_SRCS})

target_link_libraries(server libtradingcore2)
target_link_libraries(server libgsl.a)
target_link_libraries(server grpc)
target_link_libraries(server grpc++)
target_link_libraries(server grpc++_reflection)
target_link_libraries(server librocksdb.a)
target_link_libraries(server libyaml-cpp.a)