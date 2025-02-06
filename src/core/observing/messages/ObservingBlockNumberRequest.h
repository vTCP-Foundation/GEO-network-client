#ifndef VTCPD_OBSERVINGBLOCKNUMBERREQUEST_H
#define VTCPD_OBSERVINGBLOCKNUMBERREQUEST_H

#include "base/ObservingMessage.hpp"

class ObservingBlockNumberRequest : public ObservingMessage
{

public:
    typedef shared_ptr<ObservingBlockNumberRequest> Shared;

    using ObservingMessage::ObservingMessage;

    const MessageType typeID() const override;
};


#endif //VTCPD_OBSERVINGBLOCKNUMBERREQUEST_H
