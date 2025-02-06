cmake_minimum_required(VERSION 3.14)

# Add all message subdirectories
add_subdirectory(base)
add_subdirectory(trust_lines)
add_subdirectory(payments)
add_subdirectory(cycles)
add_subdirectory(max_flow_calculation)
add_subdirectory(gateway_notification_and_routing_tables)
add_subdirectory(general)
add_subdirectory(trust_line_channels)
add_subdirectory(providing)

# Main messages library
add_library(network__messages
    Message.hpp
    EquivalentMessage.h
    EquivalentMessage.cpp
    SenderMessage.cpp
    SenderMessage.h)

target_include_directories(network__messages
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(network__messages
    PRIVATE
        cxx_std_20
)

target_link_libraries(network__messages
    PUBLIC
        messages__base__transaction
        messages__base__max_flow_calculation
        messages__trust_lines
        messages__payments
        messages__cycles
        messages__max_flow_calculation
        messages__gateway_notification_and_routing_tables
        messages__general
        messages__trust_line_channels
        messages__providing
)
