#ifndef VTCPD_VOTESSTATUSREQUESTMESSAGE_H
#define VTCPD_VOTESSTATUSREQUESTMESSAGE_H

#include "../base/transaction/TransactionMessage.h"


class VotesStatusRequestMessage :
    public TransactionMessage
{

public:

    using TransactionMessage::TransactionMessage;

public:
    const MessageType typeID() const
    {
        return Message::Payments_VotesStatusRequest;
    };
};


#endif //VTCPD_VOTESSTATUSREQUESTMESSAGE_H
