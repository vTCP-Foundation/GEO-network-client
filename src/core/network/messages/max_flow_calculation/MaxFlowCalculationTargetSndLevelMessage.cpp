#include "MaxFlowCalculationTargetSndLevelMessage.h"

MaxFlowCalculationTargetSndLevelMessage::MaxFlowCalculationTargetSndLevelMessage(
    const SerializedEquivalent equivalent,
    ContractorID idOnReceiverSide,
    vector<BaseAddress::Shared> targetAddresses,
    bool isTargetGateway) : MaxFlowCalculationMessage(equivalent,
                idOnReceiverSide,
                targetAddresses),
    mIsTargetGateway(isTargetGateway)
{
}

MaxFlowCalculationTargetSndLevelMessage::MaxFlowCalculationTargetSndLevelMessage(
    BytesShared buffer) : MaxFlowCalculationMessage(buffer)
{
    size_t bytesBufferOffset = MaxFlowCalculationMessage::kOffsetToInheritedBytes();

    memcpy(
        &mIsTargetGateway,
        buffer.get() + bytesBufferOffset,
        sizeof(byte_t));
}

bool MaxFlowCalculationTargetSndLevelMessage::isTargetGateway() const
{
    return mIsTargetGateway;
}

const Message::MessageType MaxFlowCalculationTargetSndLevelMessage::typeID() const
{
    return Message::MessageType::MaxFlow_CalculationTargetSecondLevel;
}

pair<BytesShared, size_t> MaxFlowCalculationTargetSndLevelMessage::serializeToBytes() const
{
    auto parentBytesAndCount = MaxFlowCalculationMessage::serializeToBytes();
    size_t bytesCount =
        parentBytesAndCount.second +
        sizeof(byte_t);

    BytesShared dataBytesShared = tryCalloc(bytesCount);
    size_t dataBytesOffset = 0;
    //----------------------------------------------------
    memcpy(
        dataBytesShared.get(),
        parentBytesAndCount.first.get(),
        parentBytesAndCount.second);
    dataBytesOffset += parentBytesAndCount.second;
    memcpy(
        dataBytesShared.get() + dataBytesOffset,
        &mIsTargetGateway,
        sizeof(byte_t));

    return make_pair(
               dataBytesShared,
               bytesCount);
}
