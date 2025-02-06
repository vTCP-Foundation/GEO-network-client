#ifndef VTCPD_CYCLEFIVENODESINBETWEENMESSAGE_H
#define VTCPD_CYCLEFIVENODESINBETWEENMESSAGE_H

#include "base/CyclesBaseFiveOrSixNodesInBetweenMessage.h"

class CyclesFiveNodesInBetweenMessage: public CycleBaseFiveOrSixNodesInBetweenMessage
{
public:
    typedef shared_ptr<CyclesFiveNodesInBetweenMessage> Shared;

public:
    using CycleBaseFiveOrSixNodesInBetweenMessage::CycleBaseFiveOrSixNodesInBetweenMessage;

    const MessageType typeID() const override
    {
        return Message::Cycles_FiveNodesMiddleware;
    };
};
#endif //VTCPD_CYCLEFIVENODESINBETWEENMESSAGE_H