#ifndef VTCPD_MAXFLOWCALCULATIONSOURCESNDLEVELMESSAGE_H
#define VTCPD_MAXFLOWCALCULATIONSOURCESNDLEVELMESSAGE_H

#include "../base/max_flow_calculation/MaxFlowCalculationMessage.h"

class MaxFlowCalculationSourceSndLevelMessage : public MaxFlowCalculationMessage
{

public:
    typedef shared_ptr<MaxFlowCalculationSourceSndLevelMessage> Shared;

public:
    using MaxFlowCalculationMessage::MaxFlowCalculationMessage;

    const MessageType typeID() const override;
};

#endif //VTCPD_MAXFLOWCALCULATIONSOURCESNDLEVELMESSAGE_H
