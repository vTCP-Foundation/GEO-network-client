#ifndef GEO_NETWORK_CLIENT_BASETRUSTLINETRANSACTION_H
#define GEO_NETWORK_CLIENT_BASETRUSTLINETRANSACTION_H

#include "../../base/BaseTransaction.h"
#include "../../../../contractors/ContractorsManager.h"
#include "../../../../trust_lines/manager/TrustLinesManager.h"
#include "../../../../features/FeaturesManager.h"
#include "../../../../crypto/keychain.h"
#include "../../../../crypto/lamportkeys.h"

#include "../../../../subsystems_controller/TrustLinesInfluenceController.h"

#include "../../../../network/messages/trust_lines/AuditMessage.h"
#include "../../../../network/messages/trust_lines/AuditResponseMessage.h"
#include "../../../../network/messages/general/PingMessage.h"

#include "../ConflictResolverInitiatorTransaction.h"

class BaseTrustLineTransaction : public BaseTransaction
{

public:
    typedef shared_ptr<BaseTrustLineTransaction> Shared;
    BaseTrustLineTransaction(
        const TransactionType type,
        const SerializedEquivalent equivalent,
        ContractorID contractorID,
        ContractorsManager *contractorsManager,
        TrustLinesManager *trustLines,
        StorageHandler *storageHandler,
        Keystore *keystore,
        FeaturesManager *featuresManager,
        TrustLinesInfluenceController *trustLinesInfluenceController,
        Logger &log);

    BaseTrustLineTransaction(
        const TransactionType type,
        const TransactionUUID &transactionUUID,
        const SerializedEquivalent equivalent,
        ContractorID contractorID,
        ContractorsManager *contractorsManager,
        TrustLinesManager *trustLines,
        StorageHandler *storageHandler,
        Keystore *keystore,
        FeaturesManager *featuresManager,
        TrustLinesInfluenceController *trustLinesInfluenceController,
        Logger &log);

protected:
    enum Stages
    {
        Initialization = 1,
        NextAttempt = 2,
        ResponseProcessing = 3,
    };

protected:
    TransactionResult::SharedConst sendAuditErrorConfirmation(
        ConfirmationMessage::OperationState errorState);

    pair<BytesShared, size_t> getOwnSerializedAuditData(
        lamport::KeyHash::Shared ownPublicKeysHash,
        lamport::KeyHash::Shared contractorPublicKeysHash);

    pair<BytesShared, size_t> getContractorSerializedAuditData(
        lamport::KeyHash::Shared ownPublicKeysHash,
        lamport::KeyHash::Shared contractorPublicKeysHash);

protected:
    static const uint32_t kWaitMillisecondsForResponse = 20000;
    static const uint16_t kMaxCountSendingAttempts = 3;

protected:
    ContractorsManager *mContractorsManager;
    TrustLinesManager *mTrustLines;
    StorageHandler *mStorageHandler;
    Keystore *mKeysStore;
    FeaturesManager *mFeaturesManager;

    ContractorID mContractorID;
    AuditNumber mAuditNumber;
    pair<lamport::Signature::Shared, KeyNumber> mOwnSignatureAndKeyNumber;

    TrustLinesInfluenceController *mTrustLinesInfluenceController;
};


#endif //GEO_NETWORK_CLIENT_BASETRUSTLINETRANSACTION_H
