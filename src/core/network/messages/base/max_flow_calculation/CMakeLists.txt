cmake_minimum_required(VERSION 3.14)

add_library(messages__base__max_flow_calculation
    MaxFlowCalculationMessage.cpp
    MaxFlowCalculationMessage.h
    MaxFlowCalculationConfirmationMessage.h
    MaxFlowCalculationConfirmationMessage.cpp)

target_include_directories(messages__base__max_flow_calculation
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(messages__base__max_flow_calculation
    PRIVATE
        cxx_std_20
)

target_link_libraries(messages__base__max_flow_calculation
    PUBLIC
        messages__base__transaction
        common
        exceptions)