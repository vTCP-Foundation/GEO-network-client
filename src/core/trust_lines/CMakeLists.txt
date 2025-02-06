cmake_minimum_required(VERSION 3.14)

add_library(trust_lines
    manager/TrustLinesManager.cpp
    manager/TrustLinesManager.h
    audit_rules/BaseAuditRule.h
    audit_rules/BaseAuditRule.cpp
    audit_rules/AuditRuleCountPayments.h
    audit_rules/AuditRuleCountPayments.cpp
    audit_rules/AuditRuleBoundaryOverflowed.h
    audit_rules/AuditRuleBoundaryOverflowed.cpp
    TrustLine.cpp
    TrustLine.h)

target_include_directories(trust_lines
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(trust_lines
    PRIVATE
        cxx_std_20
)

target_link_libraries(trust_lines
    PUBLIC
        common
        reservations)