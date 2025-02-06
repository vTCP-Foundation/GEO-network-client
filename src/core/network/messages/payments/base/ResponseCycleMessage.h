#ifndef GEO_NETWORK_CLIENT_RESPONSECYCLEMESSAGE_H
#define GEO_NETWORK_CLIENT_RESPONSECYCLEMESSAGE_H

#include "../../base/transaction/TransactionMessage.h"

class ResponseCycleMessage : public TransactionMessage
{

public:
    enum OperationState
    {
        Accepted = 1,
        Rejected = 2,
        RejectedDueOwnKeysAbsence = 3,
        RejectedDueContractorKeysAbsence = 4,
        RejectedBecauseReservations = 5,
        NextNodeInaccessible = 6
    };

public:
    ResponseCycleMessage(
        const SerializedEquivalent equivalent,
        vector<BaseAddress::Shared> &senderAddresses,
        const TransactionUUID &transactionUUID,
        const OperationState state);

    ResponseCycleMessage(
        BytesShared buffer);

    const OperationState state() const;

protected:
    typedef byte_t SerializedOperationState;

    const size_t kOffsetToInheritedBytes() const override;

    pair<BytesShared, size_t> serializeToBytes() const override;

private:
    OperationState mState;
};

#endif // GEO_NETWORK_CLIENT_RESPONSECYCLEMESSAGE_H
