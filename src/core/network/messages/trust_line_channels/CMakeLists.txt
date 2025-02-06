cmake_minimum_required(VERSION 3.14)

add_library(messages__trust_line_channels
    InitChannelMessage.h
    InitChannelMessage.cpp
    ConfirmChannelMessage.h
    ConfirmChannelMessage.cpp
    UpdateChannelAddressesMessage.h
    UpdateChannelAddressesMessage.cpp)

target_include_directories(messages__trust_line_channels
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(messages__trust_line_channels
    PRIVATE
        cxx_std_20
)

target_link_libraries(messages__trust_line_channels
    PUBLIC
        common
        exceptions)

