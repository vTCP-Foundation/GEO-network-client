cmake_minimum_required(VERSION 3.14)

add_library(messages__providing
    ProvidingAddressResponseMessage.h
    ProvidingAddressResponseMessage.cpp)

target_include_directories(messages__providing
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(messages__providing
    PRIVATE
        cxx_std_20
)

target_link_libraries(messages__providing
    PUBLIC
        common)

