#ifndef VTCPD_PAYMENTADDITIONALRECORD_H
#define VTCPD_PAYMENTADDITIONALRECORD_H

#include "../base/Record.h"

#include "../../../../common/memory/MemoryUtils.h"
#include "../../../../common/multiprecision/MultiprecisionUtils.h"

class PaymentAdditionalRecord : public Record
{

public:
    typedef shared_ptr<PaymentAdditionalRecord> Shared;

public:
    enum PaymentAdditionalOperationType
    {
        IntermediatePaymentType = 1,
        CycleCloserType,
        CycleCloserIntermediateType,
    };
    typedef uint8_t SerializedPaymentOperationType;

    PaymentAdditionalRecord(
        const TransactionUUID &operationUUID,
        const PaymentAdditionalOperationType operationType,
        const TrustLineAmount &amount,
        vector<pair<ContractorID, TrustLineAmount>> &outgoingTransfers,
        vector<pair<ContractorID, TrustLineAmount>> &incomingTransfers);

    PaymentAdditionalRecord(
        const TransactionUUID &operationUUID,
        const GEOEpochTimestamp geoEpochTimestamp,
        BytesShared recordBody);

    const PaymentAdditionalOperationType operationType() const;

    const TrustLineAmount &amount() const;

    pair<BytesShared, size_t> serializedHistoryRecordBody() const override;

private:
    PaymentAdditionalOperationType mPaymentOperationType;
    TrustLineAmount mAmount;
    vector<pair<ContractorID, TrustLineAmount>> mOutgoingTransfers;
    vector<pair<ContractorID, TrustLineAmount>> mIncomingTransfers;
};


#endif //VTCPD_PAYMENTADDITIONALRECORD_H
