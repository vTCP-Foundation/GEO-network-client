#ifndef VTCPD_COORDINATORCYCLERESERVATIONRESPONSEMESSAGE_H
#define VTCPD_COORDINATORCYCLERESERVATIONRESPONSEMESSAGE_H

#include "base/ResponseCycleMessage.h"
#include "../../../common/multiprecision/MultiprecisionUtils.h"

class CoordinatorCycleReservationResponseMessage :
    public ResponseCycleMessage
{

public:
    typedef shared_ptr<CoordinatorCycleReservationResponseMessage> Shared;
    typedef shared_ptr<const CoordinatorCycleReservationResponseMessage> ConstShared;

public:
    // TODO: Amount may be used as flag for approved/rejected
    // (true if amount > 0)
    CoordinatorCycleReservationResponseMessage(
        const SerializedEquivalent equivalent,
        vector<BaseAddress::Shared> &senderAddresses,
        const TransactionUUID &transactionUUID,
        const OperationState state,
        const TrustLineAmount &reservedAmount=0);

    CoordinatorCycleReservationResponseMessage(
        BytesShared buffer);

    const TrustLineAmount& amountReserved() const;

    pair<BytesShared, size_t> serializeToBytes() const override;

    const MessageType typeID() const override;

protected:
    TrustLineAmount mAmountReserved;
};


#endif //VTCPD_COORDINATORCYCLERESERVATIONRESPONSEMESSAGE_H
