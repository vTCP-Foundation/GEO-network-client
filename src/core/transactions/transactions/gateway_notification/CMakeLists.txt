cmake_minimum_required(VERSION 3.14)

add_library(transactions__gateway_notification
    GatewayNotificationSenderTransaction.h
    GatewayNotificationSenderTransaction.cpp
    GatewayNotificationReceiverTransaction.h
    GatewayNotificationReceiverTransaction.cpp)

target_include_directories(transactions__gateway_notification
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(transactions__gateway_notification
    PRIVATE
        cxx_std_20
)

target_link_libraries(transactions__gateway_notification
    PUBLIC
        transactions__base
        messages__gateway_notification_and_routing_tables)