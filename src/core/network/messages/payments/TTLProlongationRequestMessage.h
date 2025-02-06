#ifndef VTCPD_TTLPROLONGATIONREQUESTMESSAGE_H
#define VTCPD_TTLPROLONGATIONREQUESTMESSAGE_H

#include "../base/transaction/TransactionMessage.h"

class TTLProlongationRequestMessage : public TransactionMessage
{

public:
    typedef shared_ptr<TTLProlongationRequestMessage> Shared;
    typedef shared_ptr<const TTLProlongationRequestMessage> ConstShared;

public:
    using TransactionMessage::TransactionMessage;

    const MessageType typeID() const override;
};


#endif //VTCPD_TTLPROLONGATIONREQUESTMESSAGE_H
