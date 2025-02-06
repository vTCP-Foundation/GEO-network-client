#include "AuditMessage.h"

AuditMessage::AuditMessage(
    const SerializedEquivalent equivalent,
    Contractor::Shared contractor,
    const TransactionUUID &transactionUUID,
    const AuditNumber auditNumber,
    const TrustLineAmount &incomingAmount,
    const TrustLineAmount &outgoingAmount,
    const KeyNumber keyNumber,
    const lamport::Signature::Shared signature) : TransactionMessage(equivalent,
                contractor->ownIdOnContractorSide(),
                transactionUUID),
    mAuditNumber(auditNumber),
    mIncomingAmount(incomingAmount),
    mOutgoingAmount(outgoingAmount),
    mSignature(signature),
    mKeyNumber(keyNumber)
{
    encrypt(contractor);
}

AuditMessage::AuditMessage(
    BytesShared buffer) : TransactionMessage(buffer)
{
    auto bytesBufferOffset = TransactionMessage::kOffsetToInheritedBytes();

    memcpy(
        &mAuditNumber,
        buffer.get() + bytesBufferOffset,
        sizeof(AuditNumber));
    bytesBufferOffset += sizeof(AuditNumber);

    vector<byte_t> incomingAmountBytes(
        buffer.get() + bytesBufferOffset,
        buffer.get() + bytesBufferOffset + kTrustLineAmountBytesCount);
    mIncomingAmount = bytesToTrustLineAmount(incomingAmountBytes);
    bytesBufferOffset += kTrustLineAmountBytesCount;

    vector<byte_t> outgoingAmountBytes(
        buffer.get() + bytesBufferOffset,
        buffer.get() + bytesBufferOffset + kTrustLineAmountBytesCount);
    mOutgoingAmount = bytesToTrustLineAmount(outgoingAmountBytes);
    bytesBufferOffset += kTrustLineAmountBytesCount;

    memcpy(
        &mKeyNumber,
        buffer.get() + bytesBufferOffset,
        sizeof(KeyNumber));
    bytesBufferOffset += sizeof(KeyNumber);

    mSignature = make_shared<lamport::Signature>(
                     buffer.get() + bytesBufferOffset);
}

const Message::MessageType AuditMessage::typeID() const
{
    return Message::TrustLines_Audit;
}

const AuditNumber AuditMessage::auditNumber() const
{
    return mAuditNumber;
}

const TrustLineAmount &AuditMessage::incomingAmount() const
{
    return mIncomingAmount;
}

const TrustLineAmount &AuditMessage::outgoingAmount() const
{
    return mOutgoingAmount;
}

const uint32_t AuditMessage::keyNumber() const
{
    return mKeyNumber;
}

const lamport::Signature::Shared AuditMessage::signature() const
{
    return mSignature;
}

const bool AuditMessage::isCheckCachedResponse() const
{
    return true;
}

pair<BytesShared, size_t> AuditMessage::serializeToBytes() const
{
    const auto parentBytesAndCount = TransactionMessage::serializeToBytes();
    auto kBufferSize = parentBytesAndCount.second + sizeof(AuditNumber) + kTrustLineAmountBytesCount + kTrustLineAmountBytesCount + sizeof(KeyNumber) + mSignature->signatureSize();
    BytesShared buffer = tryMalloc(kBufferSize);

    size_t dataBytesOffset = 0;
    // Parent message content
    memcpy(
        buffer.get(),
        parentBytesAndCount.first.get(),
        parentBytesAndCount.second);
    dataBytesOffset += parentBytesAndCount.second;

    memcpy(
        buffer.get() + dataBytesOffset,
        &mAuditNumber,
        sizeof(AuditNumber));
    dataBytesOffset += sizeof(AuditNumber);

    vector<byte_t> incomingAmountBuffer = trustLineAmountToBytes(mIncomingAmount);
    memcpy(
        buffer.get() + dataBytesOffset,
        incomingAmountBuffer.data(),
        incomingAmountBuffer.size());
    dataBytesOffset += kTrustLineAmountBytesCount;

    vector<byte_t> outgoingAmountBuffer = trustLineAmountToBytes(mOutgoingAmount);
    memcpy(
        buffer.get() + dataBytesOffset,
        outgoingAmountBuffer.data(),
        outgoingAmountBuffer.size());
    dataBytesOffset += kTrustLineAmountBytesCount;

    memcpy(
        buffer.get() + dataBytesOffset,
        &mKeyNumber,
        sizeof(KeyNumber));
    dataBytesOffset += sizeof(KeyNumber);

    memcpy(
        buffer.get() + dataBytesOffset,
        mSignature->data(),
        mSignature->signatureSize());

    return make_pair(
               buffer,
               kBufferSize);
}

const size_t AuditMessage::kOffsetToInheritedBytes() const
{
    const auto kOffset =
        TransactionMessage::kOffsetToInheritedBytes() + sizeof(AuditNumber) + kTrustLineAmountBytesCount + kTrustLineAmountBytesCount + sizeof(KeyNumber) + mSignature->signatureSize();
    return kOffset;
}