cmake_minimum_required(VERSION 3.14)

add_library(transactions__result
    state/TransactionState.cpp
    state/TransactionState.h
    TransactionResult.cpp
    TransactionResult.h)

target_include_directories(transactions__result
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(transactions__result
    PRIVATE
        cxx_std_20
)

target_link_libraries(transactions__result
    PUBLIC
        common
        exceptions)
