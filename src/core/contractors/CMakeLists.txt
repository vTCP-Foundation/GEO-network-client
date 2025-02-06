cmake_minimum_required(VERSION 3.14)

add_library(contractors
    Contractor.h
    Contractor.cpp
    ContractorsManager.h
    ContractorsManager.cpp
    addresses/BaseAddress.h
    addresses/BaseAddress.cpp
    addresses/IPv4WithPortAddress.h
    addresses/IPv4WithPortAddress.cpp
    addresses/GNSAddress.h
    addresses/GNSAddress.cpp)

target_include_directories(contractors
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(contractors
    PRIVATE
        cxx_std_20
)

target_link_libraries(contractors
    PUBLIC
        common)