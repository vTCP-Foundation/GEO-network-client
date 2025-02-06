cmake_minimum_required(VERSION 3.14)

add_library(delayed_tasks
    TopologyCacheUpdateDelayedTask.h
    TopologyCacheUpdateDelayedTask.cpp
    GatewayNotificationAndRoutingTablesDelayedTask.h
    GatewayNotificationAndRoutingTablesDelayedTask.cpp
    TopologyEventDelayedTask.h
    TopologyEventDelayedTask.cpp)

target_include_directories(delayed_tasks
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(delayed_tasks
    PRIVATE
        cxx_std_20
)