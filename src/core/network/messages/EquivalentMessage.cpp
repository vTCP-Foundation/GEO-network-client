#include "EquivalentMessage.h"

EquivalentMessage::EquivalentMessage(
    const SerializedEquivalent equivalent):
    mEquivalent(equivalent)
{}

EquivalentMessage::EquivalentMessage(
    BytesShared buffer)
{
    memcpy(
        &mEquivalent,
        buffer.get() + Message::kOffsetToInheritedBytes(),
        sizeof(SerializedEquivalent));
}

const SerializedEquivalent EquivalentMessage::equivalent() const
{
    return mEquivalent;
}

pair<BytesShared, size_t> EquivalentMessage::serializeToBytes() const
{
    auto parentBytesAndCount = Message::serializeToBytes();

    size_t bytesCount = parentBytesAndCount.second
                        + sizeof(SerializedEquivalent);

    BytesShared dataBytesShared = tryMalloc(bytesCount);
    size_t dataBytesOffset = 0;
    //----------------------------------------------------
    memcpy(
        dataBytesShared.get(),
        parentBytesAndCount.first.get(),
        parentBytesAndCount.second);
    dataBytesOffset += parentBytesAndCount.second;
    //----------------------------------------------------
    memcpy(
        dataBytesShared.get() + dataBytesOffset,
        &mEquivalent,
        sizeof(SerializedEquivalent));
    //----------------------------------------------------
    return make_pair(
               dataBytesShared,
               bytesCount);
}

const size_t EquivalentMessage::kOffsetToInheritedBytes() const
{
    const auto kOffset =
        Message::kOffsetToInheritedBytes()
        + sizeof(SerializedEquivalent);
    return kOffset;
}
