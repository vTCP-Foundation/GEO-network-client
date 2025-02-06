cmake_minimum_required(VERSION 3.14)

project(sqlite3 C)

add_library(libsqlite3
    sqlite3.c
    sqlite3.h
    sqlite3ext.h)

target_include_directories(libsqlite3
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# SQLite might need threading support
find_package(Threads REQUIRED)

target_link_libraries(libsqlite3
    PRIVATE
        ${CMAKE_DL_LIBS}
        Threads::Threads
)

# Set compile definitions for SQLite
target_compile_definitions(libsqlite3
    PRIVATE
        SQLITE_THREADSAFE=1
        SQLITE_ENABLE_FTS3
        SQLITE_ENABLE_FTS3_PARENTHESIS
        SQLITE_ENABLE_JSON1
)

# Disable warnings for SQLite as it's a third-party library
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(libsqlite3 PRIVATE -w)
endif()
