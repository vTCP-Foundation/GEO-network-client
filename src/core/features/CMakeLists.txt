cmake_minimum_required(VERSION 3.14)

add_library(features
    FeaturesManager.h
    FeaturesManager.cpp)

target_include_directories(features
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(features
    PRIVATE
        cxx_std_20
)

target_link_libraries(features)