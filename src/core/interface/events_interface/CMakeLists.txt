cmake_minimum_required(VERSION 3.14)

add_library(interface__events
    interface/EventsInterface.h
    interface/EventsInterface.cpp
    events/Event.h
    events/Event.cpp
    interface/EventsInterfaceManager.h
    interface/EventsInterfaceManager.cpp)

target_include_directories(interface__events
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src/core/interface
)

target_compile_features(interface__events
    PRIVATE
        cxx_std_20
)

target_link_libraries(interface__events
    PUBLIC
        common
        exceptions)
