cmake_minimum_required(VERSION 3.14)

add_library(transactions__total_balances
    TotalBalancesTransaction.h
    TotalBalancesTransaction.cpp)

target_include_directories(transactions__total_balances
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(transactions__total_balances
    PRIVATE
        cxx_std_20
)

target_link_libraries(transactions__total_balances
    PUBLIC
        transactions__base
        io__storage)