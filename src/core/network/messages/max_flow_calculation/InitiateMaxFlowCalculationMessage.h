#ifndef VTCPD_INITIATEMAXFLOWCALCULATIONMESSAGE_H
#define VTCPD_INITIATEMAXFLOWCALCULATIONMESSAGE_H

#include "../SenderMessage.h"


class InitiateMaxFlowCalculationMessage :
    public SenderMessage
{

public:
    typedef shared_ptr<InitiateMaxFlowCalculationMessage> Shared;

public:
    InitiateMaxFlowCalculationMessage(
        const SerializedEquivalent equivalent,
        vector<BaseAddress::Shared> &senderAddresses,
        bool isSenderGateway);

    InitiateMaxFlowCalculationMessage(
        BytesShared buffer);

    bool isSenderGateway() const;

    const MessageType typeID() const override;

    pair<BytesShared, size_t> serializeToBytes() const override;

private:
    bool mIsSenderGateway;
};


#endif //VTCPD_INITIATEMAXFLOWCALCULATIONMESSAGE_H
