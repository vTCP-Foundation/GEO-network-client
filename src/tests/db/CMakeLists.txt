cmake_minimum_required(VERSION 3.14)

add_library(db_tests)

target_include_directories(db_tests
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(db_tests
    PRIVATE
        cxx_std_20
)

target_link_libraries(db_tests
    PUBLIC
        common
        exceptions)