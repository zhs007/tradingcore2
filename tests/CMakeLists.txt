cmake_minimum_required(VERSION 3.0.0)

enable_testing()

include_directories(../include)
include_directories(../src)
include_directories(../deps/googletest/googletest/include)

link_directories(../deps/googletest/build/lib)

aux_source_directory(. DIR_TESTS_SRCS)

add_executable(tests ${DIR_TESTS_SRCS})

#target_link_libraries(tests libgtest.a)
target_link_libraries(tests gtest)
target_link_libraries(tests libtradingcore2)

add_test(
    NAME tests
    COMMAND tests
)