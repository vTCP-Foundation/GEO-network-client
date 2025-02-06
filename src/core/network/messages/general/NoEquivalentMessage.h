#ifndef VTCPD_NOEQUIVALENTMESSAGE_H
#define VTCPD_NOEQUIVALENTMESSAGE_H

#include "../base/transaction/TransactionMessage.h"

class NoEquivalentMessage : public TransactionMessage
{

public:
    typedef shared_ptr<NoEquivalentMessage> Shared;

public:
    using TransactionMessage::TransactionMessage;

    const MessageType typeID() const override;
};


#endif //VTCPD_NOEQUIVALENTMESSAGE_H
