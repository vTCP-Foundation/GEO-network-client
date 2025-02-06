cmake_minimum_required(VERSION 3.14)

add_library(messages__general
    PingMessage.h
    PingMessage.cpp
    PongMessage.h
    PongMessage.cpp
    NoEquivalentMessage.h
    NoEquivalentMessage.cpp)

target_include_directories(messages__general
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(messages__general
    PRIVATE
        cxx_std_20
)