#ifndef VTCPD_MAXFLOWCALCULATIONSOURCEFSTLEVELMESSAGE_H
#define VTCPD_MAXFLOWCALCULATIONSOURCEFSTLEVELMESSAGE_H

#include "../SenderMessage.h"

class MaxFlowCalculationSourceFstLevelMessage:
    public SenderMessage
{

public:
    typedef shared_ptr<MaxFlowCalculationSourceFstLevelMessage> Shared;

public:
    using SenderMessage::SenderMessage;

    const MessageType typeID() const override;
};

#endif //VTCPD_MAXFLOWCALCULATIONSOURCEFSTLEVELMESSAGE_H
