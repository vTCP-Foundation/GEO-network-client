#include "IntermediateNodeCycleReservationResponseMessage.h"

IntermediateNodeCycleReservationResponseMessage::IntermediateNodeCycleReservationResponseMessage(
    const SerializedEquivalent equivalent,
    vector<BaseAddress::Shared> &senderAddresses,
    const TransactionUUID &transactionUUID,
    const ResponseCycleMessage::OperationState state,
    const TrustLineAmount &reservedAmount) :

    ResponseCycleMessage(
        equivalent,
        senderAddresses,
        transactionUUID,
        state),
    mAmountReserved(reservedAmount)
{
}

IntermediateNodeCycleReservationResponseMessage::IntermediateNodeCycleReservationResponseMessage(
    BytesShared buffer) :

    ResponseCycleMessage(
        buffer)
{
    auto parentMessageOffset = ResponseCycleMessage::kOffsetToInheritedBytes();
    auto amountOffset = buffer.get() + parentMessageOffset;
    auto amountEndOffset = amountOffset + kTrustLineAmountBytesCount; // TODO: deserialize only non-zero
    vector<byte_t> amountBytes(
        amountOffset,
        amountEndOffset);

    mAmountReserved = bytesToTrustLineAmount(amountBytes);
}

const TrustLineAmount &IntermediateNodeCycleReservationResponseMessage::amountReserved() const
{
    return mAmountReserved;
}

pair<BytesShared, size_t> IntermediateNodeCycleReservationResponseMessage::serializeToBytes() const
{
    auto parentBytesAndCount = ResponseCycleMessage::serializeToBytes();
    auto serializedAmount = trustLineAmountToBytes(mAmountReserved);

    size_t bytesCount =
        parentBytesAndCount.second + serializedAmount.size();

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
        serializedAmount.data(),
        serializedAmount.size());
    //----------------------------------------------------
    return make_pair(
               dataBytesShared,
               bytesCount);
}

const Message::MessageType IntermediateNodeCycleReservationResponseMessage::typeID() const
{
    return Message::Payments_IntermediateNodeCycleReservationResponse;
}