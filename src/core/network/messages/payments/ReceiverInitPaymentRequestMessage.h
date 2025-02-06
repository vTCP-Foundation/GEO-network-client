#ifndef VTCPD_RECEIVERINITPAYMENTREQUESTMESSAGE_H
#define VTCPD_RECEIVERINITPAYMENTREQUESTMESSAGE_H

#include "base/RequestMessage.h"

class ReceiverInitPaymentRequestMessage:
    public RequestMessage
{

public:
    typedef shared_ptr<ReceiverInitPaymentRequestMessage> Shared;
    typedef shared_ptr<const ReceiverInitPaymentRequestMessage> ConstShared;

public:
    ReceiverInitPaymentRequestMessage(
        const SerializedEquivalent equivalent,
        vector<BaseAddress::Shared> &senderAddresses,
        const TransactionUUID &transactionUUID,
        const TrustLineAmount &amount,
        const string payload);

    ReceiverInitPaymentRequestMessage(
        BytesShared buffer);

    const MessageType typeID() const override;

    const string payload() const;

    pair<BytesShared, size_t> serializeToBytes() const override;

private:
    string mPayload;
};
#endif //VTCPD_RECEIVERINITPAYMENTMESSAGE_H
