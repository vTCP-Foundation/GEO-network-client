#ifndef VTCPD_CYCLESIXNODESINBETWEENMESSAGE_H
#define VTCPD_CYCLESIXNODESINBETWEENMESSAGE_H

#include "base/CyclesBaseFiveOrSixNodesInBetweenMessage.h"

class CyclesSixNodesInBetweenMessage: public CycleBaseFiveOrSixNodesInBetweenMessage
{
public:
    typedef shared_ptr<CyclesSixNodesInBetweenMessage> Shared;

public:
    using CycleBaseFiveOrSixNodesInBetweenMessage::CycleBaseFiveOrSixNodesInBetweenMessage;

    const MessageType typeID() const override
    {
        return Message::Cycles_SixNodesMiddleware;
    };
};
#endif //VTCPD_CYCLESIXNODESINBETWEENMESSAGE_H