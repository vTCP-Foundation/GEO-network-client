#ifndef VTCPD_TRANSACTIONPUBLICKEYHASHMESSAGE_H
#define VTCPD_TRANSACTIONPUBLICKEYHASHMESSAGE_H

#include "../base/transaction/TransactionMessage.h"
#include "../../../crypto/lamportscheme.h"

using namespace crypto;

class TransactionPublicKeyHashMessage : public TransactionMessage
{

public:
    typedef shared_ptr<TransactionPublicKeyHashMessage> Shared;

public:
    TransactionPublicKeyHashMessage(
        const SerializedEquivalent equivalent,
        vector<BaseAddress::Shared> &senderAddresses,
        const TransactionUUID &transactionUUID,
        const PaymentNodeID paymentNodeID,
        const lamport::KeyHash::Shared transactionPublicKeyHash);

    TransactionPublicKeyHashMessage(
        const SerializedEquivalent equivalent,
        vector<BaseAddress::Shared> &senderAddresses,
        const TransactionUUID &transactionUUID,
        const PaymentNodeID paymentNodeID,
        const lamport::KeyHash::Shared transactionPublicKeyHash,
        const KeyNumber publicKeyNumber,
        const lamport::Signature::Shared signature);

    TransactionPublicKeyHashMessage(
        BytesShared buffer);

    const MessageType typeID() const override;

    const PaymentNodeID paymentNodeID() const;

    const lamport::KeyHash::Shared transactionPublicKeyHash() const;

    bool isReceiptContains() const;

    const KeyNumber publicKeyNumber() const;

    const lamport::Signature::Shared signature() const;

    pair<BytesShared, size_t> serializeToBytes() const override;

private:
    PaymentNodeID mPaymentNodeID;
    lamport::KeyHash::Shared mTransactionPublicKeyHash;
    bool mIsReceiptContains;
    KeyNumber mPublicKeyNumber;
    lamport::Signature::Shared mSignature;
};


#endif //VTCPD_TRANSACTIONPUBLICKEYHASHMESSAGE_H
