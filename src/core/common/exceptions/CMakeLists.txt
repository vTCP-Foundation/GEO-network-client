cmake_minimum_required(VERSION 3.14)

add_library(exceptions
    Exception.cpp
    Exception.h
    CommandParsingError.h
    ConflictError.h
    IndexError.h
    IOError.h
    MemoryError.h
    NotFoundError.h
    OverflowError.h
    PreconditionFailedError.h
    RuntimeError.h
    ValueError.h
    CallChainBreakException.h)

target_include_directories(exceptions
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(exceptions
    PRIVATE
        cxx_std_20
)