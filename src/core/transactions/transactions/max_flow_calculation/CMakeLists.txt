cmake_minimum_required(VERSION 3.14)

add_library(transactions__max_flow_calculation
    InitiateMaxFlowCalculationTransaction.h
    InitiateMaxFlowCalculationTransaction.cpp
    ReceiveMaxFlowCalculationOnTargetTransaction.h
    ReceiveMaxFlowCalculationOnTargetTransaction.cpp
    MaxFlowCalculationSourceFstLevelTransaction.h
    MaxFlowCalculationSourceFstLevelTransaction.cpp
    MaxFlowCalculationTargetFstLevelTransaction.h
    MaxFlowCalculationTargetFstLevelTransaction.cpp
    MaxFlowCalculationSourceSndLevelTransaction.h
    MaxFlowCalculationSourceSndLevelTransaction.cpp
    MaxFlowCalculationTargetSndLevelTransaction.h
    MaxFlowCalculationTargetSndLevelTransaction.cpp
    CollectTopologyTransaction.h
    CollectTopologyTransaction.cpp
    MaxFlowCalculationFullyTransaction.h
    MaxFlowCalculationFullyTransaction.cpp)

target_include_directories(transactions__max_flow_calculation
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(transactions__max_flow_calculation
    PRIVATE
        cxx_std_20
)

target_link_libraries(transactions__max_flow_calculation
    PUBLIC
        transactions__base
        topology
        messages__max_flow_calculation)