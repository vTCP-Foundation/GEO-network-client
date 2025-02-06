#ifndef VTCPD_CYCLESFOURNODESPOSITIVEBALANCEREQUESTMESSAGE_H
#define VTCPD_CYCLESFOURNODESPOSITIVEBALANCEREQUESTMESSAGE_H

#include "CyclesFourNodesNegativeBalanceRequestMessage.h"

class CyclesFourNodesPositiveBalanceRequestMessage :
    public CyclesFourNodesNegativeBalanceRequestMessage
{

public:
    typedef shared_ptr<CyclesFourNodesPositiveBalanceRequestMessage> Shared;

public:
    using CyclesFourNodesNegativeBalanceRequestMessage::CyclesFourNodesNegativeBalanceRequestMessage;

    const MessageType typeID() const override;
};


#endif //VTCPD_CYCLESFOURNODESPOSITIVEBALANCEREQUESTMESSAGE_H
