cmake_minimum_required(VERSION 3.14)

add_library(messages__gateway_notification_and_routing_tables
    GatewayNotificationMessage.h
    GatewayNotificationMessage.cpp
    RoutingTableResponseMessage.h
    RoutingTableResponseMessage.cpp)

target_include_directories(messages__gateway_notification_and_routing_tables
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(messages__gateway_notification_and_routing_tables
    PRIVATE
        cxx_std_20
)

target_link_libraries(messages__gateway_notification_and_routing_tables
    PUBLIC
        messages__base__transaction
        common
        exceptions)
