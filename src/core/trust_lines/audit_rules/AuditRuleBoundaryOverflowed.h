#ifndef VTCPD_AUDITRULEBOUNDARYOVERFLOWED_H
#define VTCPD_AUDITRULEBOUNDARYOVERFLOWED_H

#include "BaseAuditRule.h"

class AuditRuleBoundaryOverflowed : public BaseAuditRule
{
public:
    typedef shared_ptr<AuditRuleBoundaryOverflowed> Shared;

public:
    AuditRuleBoundaryOverflowed();

    bool check(
        TrustLine::Shared trustLine,
        IOTransaction::Shared ioTransaction=nullptr) override;

    void reset() override;
};


#endif //VTCPD_AUDITRULEBOUNDARYOVERFLOWED_H
