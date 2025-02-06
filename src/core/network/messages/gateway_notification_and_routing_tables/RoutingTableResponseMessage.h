#ifndef GEO_NETWORK_CLIENT_ROUTINGTABLERESPONCEMESSAGE_H
#define GEO_NETWORK_CLIENT_ROUTINGTABLERESPONCEMESSAGE_H

#include "../base/transaction/ConfirmationMessage.h"
#include <vector>

class RoutingTableResponseMessage :
    public ConfirmationMessage
{

public:
    typedef shared_ptr<RoutingTableResponseMessage> Shared;

public:
    RoutingTableResponseMessage(
        ContractorID idOnReceiverSide,
        const TransactionUUID &transactionUUID,
        vector<pair<SerializedEquivalent, vector<BaseAddress::Shared>>> neighbors);

    RoutingTableResponseMessage(
        BytesShared buffer);

    const MessageType typeID() const override;

    pair<BytesShared, size_t> serializeToBytes() const override;

    vector<pair<SerializedEquivalent, vector<BaseAddress::Shared>>> neighborsByEquivalents() const;

protected:
    vector<pair<SerializedEquivalent, vector<BaseAddress::Shared>>> mNeighbors;
};

#endif //GEO_NETWORK_CLIENT_ROUTINGTABLERESPONCEMESSAGE_H
