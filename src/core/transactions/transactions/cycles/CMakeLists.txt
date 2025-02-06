cmake_minimum_required(VERSION 3.14)

add_library(transactions__cycles
    FiveAndSixNodes/base/CyclesBaseFiveSixNodesInitTransaction.cpp
    FiveAndSixNodes/base/CyclesBaseFiveSixNodesInitTransaction.h
    FiveAndSixNodes/CyclesFiveNodesInitTransaction.cpp
    FiveAndSixNodes/CyclesFiveNodesInitTransaction.h
    FiveAndSixNodes/CyclesSixNodesInitTransaction.cpp
    FiveAndSixNodes/CyclesSixNodesInitTransaction.h
    FiveAndSixNodes/CyclesFiveNodesReceiverTransaction.cpp
    FiveAndSixNodes/CyclesFiveNodesReceiverTransaction.h
    FiveAndSixNodes/CyclesSixNodesReceiverTransaction.h
    FiveAndSixNodes/CyclesSixNodesReceiverTransaction.cpp
    FourNodes/CyclesFourNodesInitTransaction.h
    FourNodes/CyclesFourNodesInitTransaction.cpp
    FourNodes/CyclesFourNodesReceiverTransaction.h
    FourNodes/CyclesFourNodesReceiverTransaction.cpp
    ThreeNodes/CyclesThreeNodesInitTransaction.cpp
    ThreeNodes/CyclesThreeNodesInitTransaction.h
    ThreeNodes/CyclesThreeNodesReceiverTransaction.cpp
    ThreeNodes/CyclesThreeNodesReceiverTransaction.h)

target_include_directories(transactions__cycles
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(transactions__cycles
    PRIVATE
        cxx_std_20
)

target_link_libraries(transactions__cycles
    PUBLIC
        transactions__base
        paths)
