include(CMakeToolsHelpers OPTIONAL)

cmake_minimum_required(VERSION 3.0.0)
project(libtradingcore2 VERSION 0.1.0)

aux_source_directory(. DIR_LIB_SRCS)
aux_source_directory(./exchange DIR_LIB_EXCHANGE_SRCS)
aux_source_directory(./indicator DIR_LIB_INDICATOR_SRCS)

add_library(libtradingcore2 ${DIR_LIB_SRCS} ${DIR_LIB_EXCHANGE_SRCS} ${DIR_LIB_INDICATOR_SRCS})