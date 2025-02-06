#include "SenderMessage.h"

SenderMessage::SenderMessage(
    const SerializedEquivalent equivalent,
    ContractorID idOnReceiverSide) : EquivalentMessage(equivalent),
    idOnReceiverSide(idOnReceiverSide),
    senderAddresses({})
{
}

SenderMessage::SenderMessage(
    const SerializedEquivalent equivalent,
    vector<BaseAddress::Shared> &senderAddresses) : EquivalentMessage(equivalent),
    senderAddresses(senderAddresses)
{
}

SenderMessage::SenderMessage(
    BytesShared buffer) : EquivalentMessage(buffer)
{
    auto bytesBufferOffset = EquivalentMessage::kOffsetToInheritedBytes();

    memcpy(
        &idOnReceiverSide,
        buffer.get() + bytesBufferOffset,
        sizeof(ContractorID));
    bytesBufferOffset += sizeof(ContractorID);

    byte_t senderAddressesCnt;
    memcpy(
        &senderAddressesCnt,
        buffer.get() + bytesBufferOffset,
        sizeof(byte_t));
    bytesBufferOffset += sizeof(byte_t);

    for (int idx = 0; idx < senderAddressesCnt; idx++) {
        auto senderAddress = deserializeAddress(
                                 buffer.get() + bytesBufferOffset);
        senderAddresses.push_back(senderAddress);
        bytesBufferOffset += senderAddress->serializedSize();
    }
}

pair<BytesShared, size_t> SenderMessage::serializeToBytes() const
{
    BytesSerializer serializer;

    serializer.enqueue(EquivalentMessage::serializeToBytes());
    serializer.copy(idOnReceiverSide);
    serializer.copy((byte_t)senderAddresses.size());
    for (const auto &address : senderAddresses) {
        serializer.enqueue(
            address->serializeToBytes(),
            address->serializedSize());
    }
    return serializer.collect();
}

const size_t SenderMessage::kOffsetToInheritedBytes() const
{
    auto kOffset =
        EquivalentMessage::kOffsetToInheritedBytes() + sizeof(ContractorID) + sizeof(byte_t);
    for (const auto &address : senderAddresses) {
        kOffset += address->serializedSize();
    }
    return kOffset;
}
