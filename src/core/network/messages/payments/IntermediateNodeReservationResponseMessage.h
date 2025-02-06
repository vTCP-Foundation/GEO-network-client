﻿#ifndef INTERMEDIATENODERESERVATIONRESPONSEMESSAGE_H
#define INTERMEDIATENODERESERVATIONRESPONSEMESSAGE_H


#include "base/ResponseMessage.h"
#include "../../../common/multiprecision/MultiprecisionUtils.h"


class IntermediateNodeReservationResponseMessage:
    public ResponseMessage
{

public:
    typedef shared_ptr<IntermediateNodeReservationResponseMessage> Shared;
    typedef shared_ptr<const IntermediateNodeReservationResponseMessage> ConstShared;

public:
    // TODO: Amount may be used as flag for approved/rejected
    // (true if amount > 0)
    IntermediateNodeReservationResponseMessage(
        const SerializedEquivalent equivalent,
        vector<BaseAddress::Shared> &senderAddresses,
        const TransactionUUID &transactionUUID,
        const PathID &pathID,
        const OperationState state,
        const TrustLineAmount &reservedAmount=0);

    IntermediateNodeReservationResponseMessage(
        BytesShared buffer);

    const TrustLineAmount& amountReserved() const;

    pair<BytesShared, size_t> serializeToBytes() const override;

    const MessageType typeID() const override;

protected:
    TrustLineAmount mAmountReserved;
};

#endif // INTERMEDIATENODERESERVATIONRESPONSEMESSAGE_H
