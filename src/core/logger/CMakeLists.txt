cmake_minimum_required(VERSION 3.14)

add_library(logger
    Logger.h
    Logger.cpp
    FileLogger.h
    LoggerMixin.hpp)

target_include_directories(logger
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(logger
    PRIVATE
        cxx_std_20
)

target_link_libraries(logger
    PUBLIC
        common
)
