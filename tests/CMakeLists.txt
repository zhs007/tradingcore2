cmake_minimum_required(VERSION 3.0.0)

# enable_testing()

include_directories(../include)
include_directories(../deps/googletest/googletest/include)
include_directories(../deps/grpc/third_party/protobuf/src)
include_directories(../deps/grpc/include)
# include_directories(${CMAKE_BINARY_DIR}/deps/glog)

link_directories(../deps/googletest/build/lib)
link_directories(../deps/gsl/build)
link_directories(../deps/yaml-cpp/build)
link_directories(../deps/glog/cmakebuild)

aux_source_directory(. DIR_TESTS_SRCS)
aux_source_directory(./exchange DIR_TESTS_EXCHANGE_SRCS)
aux_source_directory(./indicator DIR_TESTS_INDICATOR_SRCS)

add_executable(tests ${DIR_TESTS_SRCS} 
                     ${DIR_TESTS_EXCHANGE_SRCS} 
                     ${DIR_TESTS_INDICATOR_SRCS})

target_link_libraries(tests libgtest.a)
target_link_libraries(tests libtradingcore2)
target_link_libraries(tests libgsl.a)
target_link_libraries(tests libyaml-cpp.a)
target_link_libraries(tests libglog.a)
target_link_libraries(tests pthread)