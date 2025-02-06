#ifndef VTCPD_FINALPATHCONFIGURATIONMESSAGE_H
#define VTCPD_FINALPATHCONFIGURATIONMESSAGE_H

#include "base/RequestMessage.h"

class FinalPathConfigurationMessage : public RequestMessage
{

public:
    typedef shared_ptr<FinalPathConfigurationMessage> Shared;
    typedef shared_ptr<const FinalPathConfigurationMessage> ConstShared;

public:
    using RequestMessage::RequestMessage;

protected:
    const MessageType typeID() const override;
};


#endif //VTCPD_FINALPATHCONFIGURATIONMESSAGE_H
