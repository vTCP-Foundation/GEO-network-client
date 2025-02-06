﻿#ifndef REQUESTMESSAGE_H
#define REQUESTMESSAGE_H


#include "../../base/transaction/TransactionMessage.h"

#include "../../../../common/multiprecision/MultiprecisionUtils.h"


class RequestMessage:
    public TransactionMessage
{

public:
    RequestMessage(
        const SerializedEquivalent equivalent,
        vector<BaseAddress::Shared> &senderAddresses,
        const TransactionUUID &transactionUUID,
        const PathID &pathID,
        const TrustLineAmount &amount);

    RequestMessage(
        BytesShared buffer);

    const TrustLineAmount& amount() const;

    const PathID& pathID() const;

protected:
    virtual pair<BytesShared, size_t> serializeToBytes() const override;

    const size_t kOffsetToInheritedBytes() const override;

protected:
    TrustLineAmount mAmount;
    PathID mPathID;
};

#endif // REQUESTMESSAGE_H
