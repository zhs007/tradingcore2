cmake_minimum_required(VERSION 3.0.0)
project(tradingcore2 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

# include(CTest)
# enable_testing()

set(gflags_DIR ./deps/gflags/cmakebuild)
find_package(gflags REQUIRED)

set(glog_DIR ./deps/glog/cmakebuild)
find_package(glog REQUIRED)

include_directories(./include)
include_directories(${gflags_INCLUDE_DIRS})
include_directories(./deps/glog/cmakebuild)
include_directories(./deps/glog/src)
include_directories(./deps/grpc/third_party/protobuf/src)

link_directories(./deps/gsl/build)
link_directories(./deps/rocksdb/build)
link_directories(./deps/yaml-cpp/build)
link_directories(./deps/glog/cmakebuild)

add_subdirectory(./deps/grpc)
add_subdirectory(./src/tradingcore2)
add_subdirectory(./src/server)
add_subdirectory(./src/client)
add_subdirectory(./src/trdb2client)
add_subdirectory(./tests)
# add_subdirectory(./deps/googletest)
# add_subdirectory(./deps/grpc)
# add_subdirectory(./deps/gflags)
# add_subdirectory(./deps/glog)
# add_subdirectory(./deps/rocksdb)

add_executable(tradingcore2 src/main.cpp)

target_link_libraries(tradingcore2 libtradingcore2)
target_link_libraries(tradingcore2 grpc)
target_link_libraries(tradingcore2 grpc++)
target_link_libraries(tradingcore2 grpc++_reflection)
target_link_libraries(tradingcore2 librocksdb.a)
target_link_libraries(tradingcore2 libgsl.a)
target_link_libraries(tradingcore2 libyaml-cpp.a)
target_link_libraries(tradingcore2 libglog.a)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
