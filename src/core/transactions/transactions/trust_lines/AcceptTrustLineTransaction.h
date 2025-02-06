#ifndef VTCPD_ACCEPTTRUSTLINETRANSACTION_H
#define VTCPD_ACCEPTTRUSTLINETRANSACTION_H

#include "../base/BaseTransaction.h"

#include "../../../network/messages/trust_lines/TrustLineInitialMessage.h"
#include "../../../network/messages/trust_lines/TrustLineConfirmationMessage.h"

#include "../../../trust_lines/manager/TrustLinesManager.h"
#include "../../../contractors/ContractorsManager.h"

#include "../../../subsystems_controller/SubsystemsController.h"
#include "../../../subsystems_controller/TrustLinesInfluenceController.h"

class AcceptTrustLineTransaction : public BaseTransaction
{

public:
    typedef shared_ptr<AcceptTrustLineTransaction> Shared;

public:
    AcceptTrustLineTransaction(
        TrustLineInitialMessage::Shared message,
        ContractorsManager *contractorsManager,
        TrustLinesManager *manager,
        StorageHandler *storageHandler,
        bool iAmGateway,
        SubsystemsController *subsystemsController,
        TrustLinesInfluenceController *trustLinesInfluenceController,
        Logger &logger);

    TransactionResult::SharedConst run() override;

protected:
    void populateHistory(
        IOTransaction::Shared ioTransaction,
        TrustLineRecord::TrustLineOperationType operationType);

    const string logHeader() const override;

private:
    TransactionResult::SharedConst sendTrustLineErrorConfirmation(
        ConfirmationMessage::OperationState errorState);

protected:
    // these constants should be the same as in OpenTrustLineTransaction
    static const uint32_t kWaitMillisecondsForResponse = 20000;
    static const uint16_t kMaxCountSendingAttempts = 3;

protected:
    ContractorID mContractorID;
    string mSenderIncomingIP;

    ContractorsManager *mContractorsManager;
    TrustLinesManager *mTrustLinesManager;
    StorageHandler *mStorageHandler;

    SubsystemsController *mSubsystemsController;
    TrustLinesInfluenceController *mTrustLinesInfluenceController;

    bool mIAmGateway;
    bool mSenderIsGateway;
};


#endif //VTCPD_ACCEPTTRUSTLINETRANSACTION_H
