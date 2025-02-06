#ifndef GEO_NETWORK_CLIENT_TRUSTLINECONFIRMATIONMESSAGE_H
#define GEO_NETWORK_CLIENT_TRUSTLINECONFIRMATIONMESSAGE_H

#include "../base/transaction/ConfirmationMessage.h"

class TrustLineConfirmationMessage : public ConfirmationMessage
{

public:
    typedef shared_ptr<TrustLineConfirmationMessage> Shared;

public:
    TrustLineConfirmationMessage(
        const SerializedEquivalent equivalent,
        Contractor::Shared contractor,
        const TransactionUUID &transactionUUID,
        bool isContractorGateway,
        const OperationState state);

    TrustLineConfirmationMessage(
        BytesShared buffer);

    const MessageType typeID() const override;

    const bool isContractorGateway() const;

    pair<BytesShared, size_t> serializeToBytes() const override;

private:
    bool mIsContractorGateway;
};


#endif //GEO_NETWORK_CLIENT_TRUSTLINECONFIRMATIONMESSAGE_H
