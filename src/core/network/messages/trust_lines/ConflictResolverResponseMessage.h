#ifndef VTCPD_CONFLICTRESOLVERRESPONSEMESSAGE_H
#define VTCPD_CONFLICTRESOLVERRESPONSEMESSAGE_H

#include "../base/transaction/ConfirmationMessage.h"

class ConflictResolverResponseMessage : public ConfirmationMessage
{

public:
    typedef shared_ptr<ConflictResolverResponseMessage> Shared;

public:
    using ConfirmationMessage::ConfirmationMessage;

    const MessageType typeID() const override;
};


#endif //VTCPD_CONFLICTRESOLVERRESPONSEMESSAGE_H
