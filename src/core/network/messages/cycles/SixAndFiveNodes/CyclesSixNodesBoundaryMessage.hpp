#ifndef VTCPD_CYCLESIXNODESBOUNDARYMESSAGE_H
#define VTCPD_CYCLESIXNODESBOUNDARYMESSAGE_H

#include "base/CyclesBaseFiveOrSixNodesBoundaryMessage.h"

class CyclesSixNodesBoundaryMessage:
    public CyclesBaseFiveOrSixNodesBoundaryMessage
{
public:
    typedef shared_ptr<CyclesSixNodesBoundaryMessage> Shared;

public:
    using CyclesBaseFiveOrSixNodesBoundaryMessage::CyclesBaseFiveOrSixNodesBoundaryMessage;

    const MessageType typeID() const override
    {
        return Message::Cycles_SixNodesBoundary;
    };
};

#endif //VTCPD_CYCLESIXNODESBOUNDARYMESSAGE_H
