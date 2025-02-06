﻿#ifndef RESPONSEMESSAGE_H
#define RESPONSEMESSAGE_H

#include "../../base/transaction/TransactionMessage.h"

class ResponseMessage : public TransactionMessage
{

public:
    enum OperationState
    {
        Accepted = 1,
        Rejected = 2,
        RejectedDueOwnKeysAbsence = 3,
        RejectedDueContractorKeysAbsence = 4,
        // used for immediately closing transaction
        Closed = 5,
        NextNodeInaccessible = 6
    };

public:
    ResponseMessage(
        const SerializedEquivalent equivalent,
        vector<BaseAddress::Shared> &senderAddresses,
        const TransactionUUID &transactionUUID,
        const PathID &pathID,
        const OperationState state);

    ResponseMessage(
        BytesShared buffer);

    const OperationState state() const;

    const PathID pathID() const;

protected:
    typedef byte_t SerializedOperationState;

    const size_t kOffsetToInheritedBytes() const override;

    pair<BytesShared, size_t> serializeToBytes() const override;

private:
    OperationState mState;
    PathID mPathID;
};

#endif // RESPONSEMESSAGE_H
