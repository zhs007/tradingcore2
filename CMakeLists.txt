cmake_minimum_required(VERSION 3.0.0)
project(tradingcore2 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

# include(CTest)
# enable_testing()

include_directories(./include)

link_directories(./deps/gsl/build)
link_directories(./deps/rocksdb/build)
link_directories(./deps/yaml-cpp/build)

add_subdirectory(./src/tradingcore2)
add_subdirectory(./src/server)
add_subdirectory(./tests)
add_subdirectory(./deps/googletest)
add_subdirectory(./deps/grpc)
# add_subdirectory(./deps/rocksdb)

add_executable(tradingcore2 src/main.cpp)

target_link_libraries(tradingcore2 libtradingcore2)
target_link_libraries(tradingcore2 grpc)
target_link_libraries(tradingcore2 grpc++)
target_link_libraries(tradingcore2 grpc++_reflection)
target_link_libraries(tradingcore2 librocksdb.a)
target_link_libraries(tradingcore2 libgsl.a)
target_link_libraries(tradingcore2 libyaml-cpp.a)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
