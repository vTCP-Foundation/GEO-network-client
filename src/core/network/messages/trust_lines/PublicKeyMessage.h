#ifndef VTCPD_PUBLICKEYMESSAGE_H
#define VTCPD_PUBLICKEYMESSAGE_H

#include "../base/transaction/TransactionMessage.h"
#include "../../../crypto/lamportkeys.h"

using namespace crypto;

class PublicKeyMessage : public TransactionMessage
{

public:
    typedef shared_ptr<PublicKeyMessage> Shared;

public:
    PublicKeyMessage(
        const SerializedEquivalent equivalent,
        Contractor::Shared contractor,
        const TransactionUUID &transactionUUID,
        const KeyNumber number,
        const lamport::PublicKey::Shared publicKey);

    PublicKeyMessage(
        BytesShared buffer);

    const KeyNumber number() const;

    const lamport::PublicKey::Shared publicKey() const;

    const MessageType typeID() const override;

    const bool isCheckCachedResponse() const override;

    pair<BytesShared, size_t> serializeToBytes() const override;

protected:
    const size_t kOffsetToInheritedBytes() const override;

private:
    KeyNumber mNumber;
    lamport::PublicKey::Shared mPublicKey;
};


#endif //VTCPD_PUBLICKEYMESSAGE_H
