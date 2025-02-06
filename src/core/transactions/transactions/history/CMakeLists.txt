cmake_minimum_required(VERSION 3.14)

add_library(transactions__history
    HistoryPaymentsTransaction.h
    HistoryPaymentsTransaction.cpp
    HistoryPaymentsAllEquivalentsTransaction.h
    HistoryPaymentsAllEquivalentsTransaction.cpp
    HistoryTrustLinesTransaction.h
    HistoryTrustLinesTransaction.cpp
    HistoryWithContractorTransaction.h
    HistoryWithContractorTransaction.cpp
    HistoryAdditionalPaymentsTransaction.h
    HistoryAdditionalPaymentsTransaction.cpp)

target_include_directories(transactions__history
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(transactions__history
    PRIVATE
        cxx_std_20
)

target_link_libraries(transactions__history
    PUBLIC
        transactions__base
        io__storage)