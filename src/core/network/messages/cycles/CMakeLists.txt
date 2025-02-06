cmake_minimum_required(VERSION 3.14)

add_library(messages__cycles
    ThreeNodes/CyclesThreeNodesBalancesRequestMessage.h
    ThreeNodes/CyclesThreeNodesBalancesRequestMessage.cpp
    ThreeNodes/CyclesThreeNodesBalancesResponseMessage.h
    ThreeNodes/CyclesThreeNodesBalancesResponseMessage.cpp
    FourNodes/CyclesFourNodesNegativeBalanceRequestMessage.h
    FourNodes/CyclesFourNodesNegativeBalanceRequestMessage.cpp
    FourNodes/CyclesFourNodesPositiveBalanceRequestMessage.h
    FourNodes/CyclesFourNodesPositiveBalanceRequestMessage.cpp
    FourNodes/CyclesFourNodesBalancesResponseMessage.h
    FourNodes/CyclesFourNodesBalancesResponseMessage.cpp
    SixAndFiveNodes/base/CyclesBaseFiveOrSixNodesInBetweenMessage.h
    SixAndFiveNodes/base/CyclesBaseFiveOrSixNodesInBetweenMessage.cpp
    SixAndFiveNodes/base/CyclesBaseFiveOrSixNodesBoundaryMessage.h
    SixAndFiveNodes/base/CyclesBaseFiveOrSixNodesBoundaryMessage.cpp
    SixAndFiveNodes/CyclesFiveNodesInBetweenMessage.hpp
    SixAndFiveNodes/CyclesSixNodesInBetweenMessage.hpp
    SixAndFiveNodes/CyclesFiveNodesBoundaryMessage.hpp
    SixAndFiveNodes/CyclesSixNodesBoundaryMessage.hpp)

target_include_directories(messages__cycles
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(messages__cycles
    PRIVATE
        cxx_std_20
)

target_link_libraries(messages__cycles
    PUBLIC
        messages__base__transaction
        common
        exceptions)
