#ifndef GEO_NETWORK_CLIENT_INTERMEDIATENODECYCLERESERVATIONRESPONSEMESSAGE_H
#define GEO_NETWORK_CLIENT_INTERMEDIATENODECYCLERESERVATIONRESPONSEMESSAGE_H

#include "base/ResponseCycleMessage.h"
#include "../../../common/multiprecision/MultiprecisionUtils.h"

class IntermediateNodeCycleReservationResponseMessage :
    public ResponseCycleMessage
{

public:
    typedef shared_ptr<IntermediateNodeCycleReservationResponseMessage> Shared;
    typedef shared_ptr<const IntermediateNodeCycleReservationResponseMessage> ConstShared;

public:
    // TODO: Amount may be used as flag for approved/rejected
    // (true if amount > 0)
    IntermediateNodeCycleReservationResponseMessage(
        const SerializedEquivalent equivalent,
        vector<BaseAddress::Shared> &senderAddresses,
        const TransactionUUID &transactionUUID,
        const OperationState state,
        const TrustLineAmount &reservedAmount=0);

    IntermediateNodeCycleReservationResponseMessage(
        BytesShared buffer);

    const TrustLineAmount& amountReserved() const;

    pair<BytesShared, size_t> serializeToBytes() const override;

    const MessageType typeID() const override;

protected:
    TrustLineAmount mAmountReserved;
};


#endif //GEO_NETWORK_CLIENT_INTERMEDIATENODECYCLERESERVATIONRESPONSEMESSAGE_H
