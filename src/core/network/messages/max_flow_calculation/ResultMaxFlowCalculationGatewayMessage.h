#ifndef VTCPD_RESULTMAXFLOWCALCULATIONGATEWAYMESSAGE_H
#define VTCPD_RESULTMAXFLOWCALCULATIONGATEWAYMESSAGE_H

#include "ResultMaxFlowCalculationMessage.h"

class ResultMaxFlowCalculationGatewayMessage :
    public ResultMaxFlowCalculationMessage
{

public:
    typedef shared_ptr<ResultMaxFlowCalculationGatewayMessage> Shared;

public:
    using ResultMaxFlowCalculationMessage::ResultMaxFlowCalculationMessage;

    const MessageType typeID() const override;
};


#endif //VTCPD_RESULTMAXFLOWCALCULATIONGATEWAYMESSAGE_H
