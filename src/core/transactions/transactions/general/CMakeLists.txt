cmake_minimum_required(VERSION 3.14)

add_library(transactions__general
    NoEquivalentTransaction.h
    NoEquivalentTransaction.cpp
    PongReactionTransaction.h
    PongReactionTransaction.cpp
    RemoveOutdatedCryptoDataTransaction.h
    RemoveOutdatedCryptoDataTransaction.cpp)

target_include_directories(transactions__general
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(transactions__general
    PRIVATE
        cxx_std_20
)

target_link_libraries(transactions__general
    PUBLIC
        transactions__base)