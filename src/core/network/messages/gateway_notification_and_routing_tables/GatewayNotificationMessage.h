#ifndef VTCPD_GATEWAYNOTIFICATIONMESSAGE_H
#define VTCPD_GATEWAYNOTIFICATIONMESSAGE_H

#include "../base/transaction/TransactionMessage.h"

class GatewayNotificationMessage : public TransactionMessage
{

public:
    typedef shared_ptr<GatewayNotificationMessage> Shared;

public:
    GatewayNotificationMessage(
        ContractorID idOnReceiverSide,
        const TransactionUUID &transactionUUID,
        const vector<SerializedEquivalent> gatewayEquivalents);

    GatewayNotificationMessage(
        BytesShared buffer);

    const vector<SerializedEquivalent> gatewayEquivalents() const;

    const MessageType typeID() const override;

    const bool isAddToConfirmationRequiredMessagesHandler() const override;

protected:
    pair<BytesShared, size_t> serializeToBytes() const override;

protected:
    vector<SerializedEquivalent> mGatewayEquivalents;
};


#endif //VTCPD_GATEWAYNOTIFICATIONMESSAGE_H
