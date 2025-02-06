﻿#ifndef GEO_NETWORK_CLIENT_CREDITUSAGECOMMAND_H
#define GEO_NETWORK_CLIENT_CREDITUSAGECOMMAND_H

#include "../BaseUserCommand.h"
#include "../../../../common/multiprecision/MultiprecisionUtils.h"
#include "../../../../common/exceptions/MemoryError.h"

class CreditUsageCommand:
    public BaseUserCommand
{

public:
    typedef shared_ptr<CreditUsageCommand> Shared;

public:
    CreditUsageCommand(
        const CommandUUID &uuid,
        const string &commandBuffer);

    static const string &identifier();

    const TrustLineAmount& amount() const;

    vector<BaseAddress::Shared> contractorAddresses() const;

    const SerializedEquivalent equivalent() const;

    const std::string payload() const;

public:
    // Results handlers
    CommandResult::SharedConst responseOK(
        string &transactionUUID) const;

private:
    vector<BaseAddress::Shared> mContractorAddresses;
    TrustLineAmount mAmount;
    SerializedEquivalent mEquivalent;
    std::string mPayload;
};

#endif //GEO_NETWORK_CLIENT_CREDITUSAGECOMMAND_H
