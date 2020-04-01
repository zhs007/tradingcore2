cmake_minimum_required(VERSION 3.0.0)

# enable_testing()

include_directories(../include)
# include_directories(../src)
include_directories(../deps/googletest/googletest/include)

# link_directories(../deps/googletest/build/lib)
link_directories(../deps/gsl/build)

aux_source_directory(. DIR_TESTS_SRCS)
aux_source_directory(./exchange DIR_TESTS_EXCHANGE_SRCS)
aux_source_directory(./indicator DIR_TESTS_INDICATOR_SRCS)

add_executable(tests ${DIR_TESTS_SRCS} 
                     ${DIR_TESTS_EXCHANGE_SRCS} 
                     ${DIR_TESTS_INDICATOR_SRCS})

target_link_libraries(tests gtest)
target_link_libraries(tests libtradingcore2)
target_link_libraries(tests libgsl.a)