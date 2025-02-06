cmake_minimum_required(VERSION 3.14)

add_library(crypto
    memory.h
    memory.cpp
    keychain.h
    keychain.cpp
    lamportkeys.cpp
    lamportkeys.h
    lamportscheme.cpp
    lamportscheme.h
    ByteEncryptor.cpp
    ByteEncryptor.h
    MsgEncryptor.cpp
    MsgEncryptor.h
    ProviderMsgEncryptor.h
    ProviderMsgEncryptor.cpp)

target_include_directories(crypto
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(crypto
    PRIVATE
        cxx_std_20
)

# Find sodium package if available, otherwise fall back to direct linking
find_package(sodium QUIET)
if(sodium_FOUND)
    target_link_libraries(crypto
        PRIVATE
            sodium::sodium
    )
else()
    target_link_libraries(crypto
        PRIVATE
            sodium
    )
endif()



