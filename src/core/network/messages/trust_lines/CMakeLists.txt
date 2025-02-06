cmake_minimum_required(VERSION 3.14)

add_library(messages__trust_lines
    TrustLineInitialMessage.h
    TrustLineInitialMessage.cpp
    PublicKeysSharingInitMessage.h
    PublicKeysSharingInitMessage.cpp
    PublicKeyMessage.h
    PublicKeyMessage.cpp
    TrustLineConfirmationMessage.h
    TrustLineConfirmationMessage.cpp
    PublicKeyHashConfirmation.h
    PublicKeyHashConfirmation.cpp
    AuditMessage.h
    AuditMessage.cpp
    AuditResponseMessage.h
    AuditResponseMessage.cpp
    ConflictResolverMessage.h
    ConflictResolverMessage.cpp
    ConflictResolverResponseMessage.h
    ConflictResolverResponseMessage.cpp
    TrustLineResetMessage.h
    TrustLineResetMessage.cpp)

target_include_directories(messages__trust_lines
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(messages__trust_lines
    PRIVATE
        cxx_std_20
)

target_link_libraries(messages__trust_lines
    PUBLIC
        common
        exceptions)

