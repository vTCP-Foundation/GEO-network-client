#ifndef VTCPD_PINGMESSAGE_H
#define VTCPD_PINGMESSAGE_H

#include "../SenderMessage.h"

class PingMessage : public SenderMessage
{

public:
    typedef shared_ptr<PingMessage> Shared;

public:
    PingMessage(
        ContractorID idOnReceiverSide);

    PingMessage(
        BytesShared buffer);

    const MessageType typeID() const override;
};


#endif //VTCPD_PINGMESSAGE_H
