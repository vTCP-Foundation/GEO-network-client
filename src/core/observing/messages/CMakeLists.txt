cmake_minimum_required(VERSION 3.14)

add_library(observing__messages
    base/ObservingMessage.hpp
    base/ObservingResponseMessage.h
    base/ObservingResponseMessage.cpp
    ObservingClaimAppendRequestMessage.h
    ObservingClaimAppendRequestMessage.cpp
    ObservingClaimAppendResponseMessage.h
    ObservingClaimAppendResponseMessage.cpp
    ObservingParticipantsVotesRequestMessage.h
    ObservingParticipantsVotesRequestMessage.cpp
    ObservingParticipantsVotesResponseMessage.h
    ObservingParticipantsVotesResponseMessage.cpp
    ObservingTransactionsRequestMessage.h
    ObservingTransactionsRequestMessage.cpp
    ObservingTransactionsResponseMessage.h
    ObservingTransactionsResponseMessage.cpp
    ObservingBlockNumberRequest.h
    ObservingBlockNumberRequest.cpp
    ObservingBlockNumberResponse.h
    ObservingBlockNumberResponse.cpp
    ObservingParticipantsVotesAppendRequestMessage.h
    ObservingParticipantsVotesAppendRequestMessage.cpp
    ObservingParticipantsVotesAppendResponseMessage.h
    ObservingParticipantsVotesAppendResponseMessage.cpp)

target_include_directories(observing__messages
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(observing__messages
    PRIVATE
        cxx_std_20
)

target_link_libraries(observing__messages
    PUBLIC
        common
        exceptions)