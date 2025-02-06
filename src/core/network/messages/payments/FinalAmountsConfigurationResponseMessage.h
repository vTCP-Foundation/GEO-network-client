#ifndef VTCPD_FINALAMOUNTSCONFIGURATIONRESPONSEMESSAGE_H
#define VTCPD_FINALAMOUNTSCONFIGURATIONRESPONSEMESSAGE_H

#include "../base/transaction/TransactionMessage.h"
#include "../../../crypto/lamportkeys.h"

using namespace crypto;

class FinalAmountsConfigurationResponseMessage : public TransactionMessage
{

public:
    enum OperationState
    {
        Accepted = 1,
        Rejected = 2,
    };

public:
    typedef shared_ptr<FinalAmountsConfigurationResponseMessage> Shared;

public:
    FinalAmountsConfigurationResponseMessage(
        const SerializedEquivalent equivalent,
        vector<BaseAddress::Shared> &senderAddresses,
        const TransactionUUID &transactionUUID,
        const OperationState state);

    FinalAmountsConfigurationResponseMessage(
        const SerializedEquivalent equivalent,
        vector<BaseAddress::Shared> &senderAddresses,
        const TransactionUUID &transactionUUID,
        const OperationState state,
        const lamport::PublicKey::Shared publicKey);

    FinalAmountsConfigurationResponseMessage(
        BytesShared buffer);

    const MessageType typeID() const override;

    const OperationState state() const;

    const lamport::PublicKey::Shared publicKey() const;

protected:
    typedef byte_t SerializedOperationState;

    pair<BytesShared, size_t> serializeToBytes() const override;

private:
    OperationState mState;
    lamport::PublicKey::Shared mPublicKey;
};

#endif // VTCPD_FINALAMOUNTSCONFIGURATIONRESPONSEMESSAGE_H
