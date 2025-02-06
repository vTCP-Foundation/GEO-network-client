#ifndef VTCPD_CYCLEFIVENODESBOUNDARYMESSAGE_H
#define VTCPD_CYCLEFIVENODESBOUNDARYMESSAGE_H

#include "base/CyclesBaseFiveOrSixNodesBoundaryMessage.h"

class CyclesFiveNodesBoundaryMessage:
    public CyclesBaseFiveOrSixNodesBoundaryMessage
{

public:
    using CyclesBaseFiveOrSixNodesBoundaryMessage::CyclesBaseFiveOrSixNodesBoundaryMessage;

    const MessageType typeID() const override
    {
        return Message::Cycles_FiveNodesBoundary;
    };
};
#endif //VTCPD_CYCLEFIVENODESBOUNDARYMESSAGE_H
