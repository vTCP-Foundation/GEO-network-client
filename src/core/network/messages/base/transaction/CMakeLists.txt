cmake_minimum_required(VERSION 3.14)

add_library(messages__base__transaction
    TransactionMessage.cpp
    TransactionMessage.h
    ConfirmationMessage.h
    ConfirmationMessage.cpp)

target_include_directories(messages__base__transaction
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(messages__base__transaction
    PRIVATE
        cxx_std_20
)

target_link_libraries(messages__base__transaction
    PUBLIC
        common
        exceptions)
