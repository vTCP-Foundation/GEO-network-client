cmake_minimum_required(VERSION 3.14)

add_library(topology
    TopologyTrustLine.h
    TopologyTrustLine.cpp
    manager/TopologyTrustLineWithPtr.h
    manager/TopologyTrustLineWithPtr.cpp
    manager/TopologyTrustLinesManager.h
    manager/TopologyTrustLinesManager.cpp
    cache/TopologyCache.h
    cache/TopologyCache.cpp
    cache/TopologyCacheManager.h
    cache/TopologyCacheManager.cpp
    cache/MaxFlowCache.h
    cache/MaxFlowCache.cpp
    cache/MaxFlowCacheManager.h
    cache/MaxFlowCacheManager.cpp)

target_include_directories(topology
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(topology
    PRIVATE
        cxx_std_20
)