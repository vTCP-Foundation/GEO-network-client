cmake_minimum_required(VERSION 3.14)

add_library(subsystems_controller
    SubsystemsController.h
    SubsystemsController.cpp
    TrustLinesInfluenceController.h
    TrustLinesInfluenceController.cpp)

target_include_directories(subsystems_controller
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(subsystems_controller
    PRIVATE
        cxx_std_20
)

target_link_libraries(subsystems_controller)