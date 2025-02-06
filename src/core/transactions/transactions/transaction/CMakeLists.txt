cmake_minimum_required(VERSION 3.14)

add_library(transactions__transaction
    PaymentTransactionByCommandUUIDTransaction.h
    PaymentTransactionByCommandUUIDTransaction.cpp)

target_include_directories(transactions__transaction
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(transactions__transaction
    PRIVATE
        cxx_std_20
)

target_link_libraries(transactions__transaction
    PUBLIC
        transactions__base
        io__storage)