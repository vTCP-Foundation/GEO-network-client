#ifndef VTCPD_OPENTRUSTLINETRANSACTION_H
#define VTCPD_OPENTRUSTLINETRANSACTION_H

#include "../base/BaseTransaction.h"
#include "../../../interface/commands_interface/commands/trust_lines/InitTrustLineCommand.h"
#include "../../../trust_lines/manager/TrustLinesManager.h"
#include "../../../contractors/ContractorsManager.h"
#include "../../../interface/events_interface/interface/EventsInterfaceManager.h"
#include "../../../subsystems_controller/TrustLinesInfluenceController.h"
#include "../../../subsystems_controller/SubsystemsController.h"

#include "../../../network/messages/trust_lines/TrustLineInitialMessage.h"
#include "../../../network/messages/trust_lines/TrustLineConfirmationMessage.h"
#include "../../../network/messages/general/PingMessage.h"

class OpenTrustLineTransaction : public BaseTransaction
{

public:
    typedef shared_ptr<OpenTrustLineTransaction> Shared;

public:
    OpenTrustLineTransaction(
        InitTrustLineCommand::Shared command,
        ContractorsManager *contractorsManager,
        TrustLinesManager *manager,
        StorageHandler *storageHandler,
        EventsInterfaceManager *eventsInterfaceManager,
        bool iAmGateway,
        SubsystemsController *subsystemsController,
        TrustLinesInfluenceController *trustLinesInfluenceController,
        Logger &logger);

    OpenTrustLineTransaction(
        const SerializedEquivalent equivalent,
        ContractorID contractorID,
        ContractorsManager *contractorsManager,
        TrustLinesManager *manager,
        StorageHandler *storageHandler,
        EventsInterfaceManager *eventsInterfaceManager,
        bool iAmGateway,
        SubsystemsController *subsystemsController,
        TrustLinesInfluenceController *trustLinesInfluenceController,
        Logger &logger);

    TransactionResult::SharedConst run() override;

protected:
    enum Stages
    {
        Initialization = 1,
        NextAttempt = 2,
        ResponseProcessing = 3,
    };

protected:
    TransactionResult::SharedConst resultOK();

    TransactionResult::SharedConst resultForbiddenRun();

    TransactionResult::SharedConst resultProtocolError();

    TransactionResult::SharedConst resultTrustLineAlreadyExist();

    TransactionResult::SharedConst resultUnexpectedError();

protected:
    void populateHistory(
        IOTransaction::Shared ioTransaction,
        TrustLineRecord::TrustLineOperationType operationType);

    const string logHeader() const override;

private:
    TransactionResult::SharedConst runInitializationStage();

    TransactionResult::SharedConst runNextAttemptStage();

    TransactionResult::SharedConst runResponseProcessingStage();

protected:
    // these constants should be the same as in AcceptTrustLineTransaction
    static const uint32_t kWaitMillisecondsForResponse = 20000;
    static const uint16_t kMaxCountSendingAttempts = 3;

protected:
    InitTrustLineCommand::Shared mCommand;
    ContractorID mContractorID;
    ContractorsManager *mContractorsManager;
    TrustLinesManager *mTrustLines;
    StorageHandler *mStorageHandler;
    EventsInterfaceManager *mEventsInterfaceManager;

    uint16_t mCountSendingAttempts;

    TrustLinesInfluenceController *mTrustLinesInfluenceController;
    SubsystemsController *mSubsystemsController;
    bool mIAmGateway;
};


#endif //VTCPD_OPENTRUSTLINETRANSACTION_H
