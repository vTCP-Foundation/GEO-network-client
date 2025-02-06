cmake_minimum_required(VERSION 3.14)

add_library(io_routing_tables_tests
    OperationsLogTests.cpp
    operations/RemoveOperationTests.cpp
    operations/DirectionUpdateOperationTests.cpp
    operations/SetOperationTests.cpp
    RoutingTableTests.cpp)

target_include_directories(io_routing_tables_tests
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(io_routing_tables_tests
    PRIVATE
        cxx_std_20
)

target_link_libraries(io_routing_tables_tests
    PUBLIC
        common
        io_routing_tables)