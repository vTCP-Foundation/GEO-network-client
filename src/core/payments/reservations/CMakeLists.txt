cmake_minimum_required(VERSION 3.14)

add_library(reservations
    AmountReservation.h
    AmountReservation.cpp
    AmountReservationsHandler.h
    AmountReservationsHandler.cpp)

target_include_directories(reservations
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(reservations
    PRIVATE
        cxx_std_20
)

target_link_libraries(reservations
    PUBLIC
        common)