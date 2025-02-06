#ifndef VTCPD_FINALPATHCYCLECONFIGURATIONMESSAGE_H
#define VTCPD_FINALPATHCYCLECONFIGURATIONMESSAGE_H

#include "base/RequestCycleMessage.h"
#include "../../../contractors/Contractor.h"
#include "../../../crypto/lamportscheme.h"
#include <map>

using namespace crypto;

class FinalPathCycleConfigurationMessage :
    public RequestCycleMessage
{

public:
    typedef shared_ptr<FinalPathCycleConfigurationMessage> Shared;

public:
    FinalPathCycleConfigurationMessage(
        const SerializedEquivalent equivalent,
        vector<BaseAddress::Shared> &senderAddresses,
        const TransactionUUID &transactionUUID,
        const TrustLineAmount &amount,
        const map<PaymentNodeID, Contractor::Shared> &paymentParticipants,
        const BlockNumber maximalClaimingBlockNumber);

    FinalPathCycleConfigurationMessage(
        const SerializedEquivalent equivalent,
        vector<BaseAddress::Shared> &senderAddresses,
        const TransactionUUID &transactionUUID,
        const TrustLineAmount &amount,
        const map<PaymentNodeID, Contractor::Shared> &paymentParticipants,
        const BlockNumber maximalClaimingBlockNumber,
        const KeyNumber publicKeyNumber,
        const lamport::Signature::Shared signature,
        const lamport::KeyHash::Shared transactionPublicKeyHash);

    FinalPathCycleConfigurationMessage(
        BytesShared buffer);

    const MessageType typeID() const override;

    const map<PaymentNodeID, Contractor::Shared>& paymentParticipants() const;

    const BlockNumber maximalClaimingBlockNumber() const;

    bool isReceiptContains() const;

    const KeyNumber publicKeyNumber() const;

    const lamport::Signature::Shared signature() const;

    const lamport::KeyHash::Shared transactionPublicKeyHash() const;

    pair<BytesShared, size_t> serializeToBytes() const override;

private:
    map<PaymentNodeID, Contractor::Shared> mPaymentParticipants;
    BlockNumber mMaximalClaimingBlockNumber;
    bool mIsReceiptContains;
    KeyNumber mPublicKeyNumber;
    lamport::Signature::Shared mSignature;
    lamport::KeyHash::Shared mTransactionPublicKeyHash;
};


#endif //VTCPD_FINALPATHCYCLECONFIGURATIONMESSAGE_H
