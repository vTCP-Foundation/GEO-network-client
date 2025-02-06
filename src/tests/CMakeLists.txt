cmake_minimum_required(VERSION 3.14)

# Enable testing
enable_testing()

# Find GTest package
find_package(GTest REQUIRED)

add_executable(unit_tests
    TestIncludes.h
    interface/сommands_interface/commands/history/HistoryAdditionalPaymentsCommandTest.cpp
    interface/сommands_interface/commands/history/HistoryPaymentsCommandTest.cpp
    interface/сommands_interface/commands/history/HistoryTrustLinesCommandTest.cpp
    interface/сommands_interface/commands/history/HistoryWithContractorCommandTest.cpp
    interface/сommands_interface/commands/max_flow_calculation/InitiateMaxFlowCalculationCommandTest.cpp
    interface/сommands_interface/commands/max_flow_calculation/InitiateMaxFlowCalculationFullyCommandTest.cpp
    interface/сommands_interface/commands/payments/CreditUsageCommandTest.cpp
    interface/сommands_interface/commands/total_balances/TotalBalancesCommandTest.cpp
    interface/сommands_interface/commands/transactions/PaymentTransactionByCommandUUIDCommandTest.cpp
    interface/сommands_interface/commands/trust_lines_list/GetFirstLevelContractorsCommandTest.cpp
    interface/сommands_interface/commands/trust_lines_list/GetTrustLineCommandTest.cpp
    interface/сommands_interface/commands/trust_lines_list/GetTrustLinesCommandTest.cpp
    interface/сommands_interface/commands/subsystems_controller/SubsystensInfluenceCommandTest.cpp
    interface/сommands_interface/commands/subsystems_controller/TrustLinesInfluenceCommandTest.cpp
    interface/сommands_interface/commands/trust_lines/CloseIncTrustLineCommandTest.cpp
    interface/сommands_interface/commands/trust_lines/InitTrustLineTest.cpp
    interface/сommands_interface/commands/trust_lines/SetOutgoingTrustLineCommandTest.cpp
    interface/сommands_interface/commands/trust_lines/ShareKeysCommandTest.cpp)

target_include_directories(unit_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${GTEST_INCLUDE_DIRS}
)

target_compile_features(unit_tests
    PRIVATE
        cxx_std_20
)

target_link_libraries(unit_tests
    PRIVATE
        GTest::GTest
        GTest::Main
)

# Add tests to CTest
add_test(NAME unit_tests COMMAND unit_tests)
