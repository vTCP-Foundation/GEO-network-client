#include "OpenTrustLineTransaction.h"

OpenTrustLineTransaction::OpenTrustLineTransaction(
    InitTrustLineCommand::Shared command,
    ContractorsManager *contractorsManager,
    TrustLinesManager *manager,
    StorageHandler *storageHandler,
    EventsInterfaceManager *eventsInterfaceManager,
    bool iAmGateway,
    SubsystemsController *subsystemsController,
    TrustLinesInfluenceController *trustLinesInfluenceController,
    Logger &logger) :

    BaseTransaction(
        BaseTransaction::OpenTrustLineTransaction,
        command->equivalent(),
        logger),
    mCommand(command),
    mCountSendingAttempts(0),
    mContractorsManager(contractorsManager),
    mTrustLines(manager),
    mStorageHandler(storageHandler),
    mEventsInterfaceManager(eventsInterfaceManager),
    mSubsystemsController(subsystemsController),
    mTrustLinesInfluenceController(trustLinesInfluenceController),
    mIAmGateway(iAmGateway)
{
    mStep = Initialization;
}

OpenTrustLineTransaction::OpenTrustLineTransaction(
    const SerializedEquivalent equivalent,
    ContractorID contractorID,
    ContractorsManager *contractorsManager,
    TrustLinesManager *manager,
    StorageHandler *storageHandler,
    EventsInterfaceManager *eventsInterfaceManager,
    bool iAmGateway,
    SubsystemsController *subsystemsController,
    TrustLinesInfluenceController *trustLinesInfluenceController,
    Logger &logger) :

    BaseTransaction(
        BaseTransaction::OpenTrustLineTransaction,
        equivalent,
        logger),
    mContractorID(contractorID),
    mCountSendingAttempts(0),
    mContractorsManager(contractorsManager),
    mTrustLines(manager),
    mStorageHandler(storageHandler),
    mEventsInterfaceManager(eventsInterfaceManager),
    mSubsystemsController(subsystemsController),
    mTrustLinesInfluenceController(trustLinesInfluenceController),
    mIAmGateway(iAmGateway)
{
    mStep = NextAttempt;
}

TransactionResult::SharedConst OpenTrustLineTransaction::run()
{
    info() << "step " << mStep;
    switch (mStep) {
    case Stages::Initialization: {
        return runInitializationStage();
    }
    case Stages::NextAttempt: {
        return runNextAttemptStage();
    }
    case Stages::ResponseProcessing: {
        return runResponseProcessingStage();
    }
    default:
        throw ValueError(logHeader() + "::run: "
                                       "wrong value of mStep");
    }
}

TransactionResult::SharedConst OpenTrustLineTransaction::runInitializationStage()
{
    if (!mSubsystemsController->isRunTrustLineTransactions()) {
        debug() << "It is forbidden run trust line transactions";
        return resultForbiddenRun();
    }
    info() << "Try init TL to Contractor: " << mCommand->contractorID();
    if (!mContractorsManager->contractorPresent(mCommand->contractorID())) {
        warning() << "There is no contractor with requested ID";
        return resultProtocolError();
    }
    if (!mContractorsManager->channelConfirmed(mCommand->contractorID())) {
        warning() << "Channel is not confirmed";
        return resultProtocolError();
    }
    mContractorID = mCommand->contractorID();
    info() << "Contractor main address: " << mContractorsManager->contractorMainAddress(mContractorID)->fullAddress();

    auto ioTransaction = mStorageHandler->beginTransaction();
    if (mTrustLines->trustLineIsPresent(mContractorID)) {
        if (mTrustLines->trustLineState(mContractorID) != TrustLine::Archived) {
            warning() << "Trust line already present.";
            return resultTrustLineAlreadyExist();
        } else {
            info() << "Reopening of archived TL";
        }
    }

    try {
        if (mTrustLines->trustLineIsPresent(mContractorID)) {
            mTrustLines->setTrustLineState(
                mContractorID,
                TrustLine::Init,
                ioTransaction);
            info() << "TrustLine to the node " << mContractorID
                   << " successfully reinitialised.";
        } else {
            mTrustLines->open(
                mContractorID,
                ioTransaction);
            info() << "TrustLine to the node " << mContractorID
                   << " successfully initialised.";
        }

#ifdef TESTS
        mTrustLinesInfluenceController->testThrowExceptionOnSourceInitializationStage(
            BaseTransaction::OpenTrustLineTransaction);
        mTrustLinesInfluenceController->testTerminateProcessOnSourceInitializationStage(
            BaseTransaction::OpenTrustLineTransaction);
#endif

    } catch (IOError &e) {
        ioTransaction->rollback();
        mTrustLines->removeTrustLine(
            mContractorID);
        error() << "Error during saving TA. Details: " << e.what();
        return resultUnexpectedError();
    }

    sendMessage<TrustLineInitialMessage>(
        mContractorID,
        mEquivalent,
        mContractorsManager->contractor(mContractorID),
        mTransactionUUID,
        mIAmGateway);
    mCountSendingAttempts++;
    info() << "Message with TL opening request was sent";

    mStep = ResponseProcessing;
    return resultOK();
}

TransactionResult::SharedConst OpenTrustLineTransaction::runNextAttemptStage()
{
    info() << "runNextAttemptStage";
    if (!mSubsystemsController->isRunTrustLineTransactions()) {
        debug() << "It is forbidden run trust line transactions";
        return resultDone();
    }
    info() << "Try init TL to " << mContractorID;
    if (!mContractorsManager->contractorPresent(mContractorID)) {
        warning() << "There is no contractor with requested id";
        return resultProtocolError();
    }
    if (!mContractorsManager->channelConfirmed(mContractorID)) {
        warning() << "Channel is not confirmed";
        return resultProtocolError();
    }

    if (!mTrustLines->trustLineIsPresent(mContractorID)) {
        warning() << "Trust line is absent.";
        return resultDone();
    }

    if (mTrustLines->trustLineState(mContractorID) != TrustLine::Init) {
        warning() << "Invalid TL state " << mTrustLines->trustLineState(mContractorID);
        return resultDone();
    }

    processPongMessage(mContractorID);

#ifdef TESTS
    mTrustLinesInfluenceController->testThrowExceptionOnSourceResumingStage(
        BaseTransaction::OpenTrustLineTransaction);
    mTrustLinesInfluenceController->testTerminateProcessOnSourceResumingStage(
        BaseTransaction::OpenTrustLineTransaction);
#endif

    sendMessage<TrustLineInitialMessage>(
        mContractorID,
        mEquivalent,
        mContractorsManager->contractor(mContractorID),
        mTransactionUUID,
        mIAmGateway);
    mCountSendingAttempts++;
    info() << "Message with TL opening request was sent";

    mStep = ResponseProcessing;
    return resultWaitForMessageTypes(
    {Message::TrustLines_Confirmation},
    kWaitMillisecondsForResponse);
}

TransactionResult::SharedConst OpenTrustLineTransaction::runResponseProcessingStage()
{
    if (mContext.empty()) {
        warning() << "Contractor don't send response.";
        if (mCountSendingAttempts < kMaxCountSendingAttempts) {
            sendMessage<TrustLineInitialMessage>(
                mContractorID,
                mEquivalent,
                mContractorsManager->contractor(mContractorID),
                mTransactionUUID,
                mIAmGateway);
            mCountSendingAttempts++;
            info() << "Send message " << mCountSendingAttempts << " times";
            return resultWaitForMessageTypes(
            {Message::TrustLines_Confirmation},
            kWaitMillisecondsForResponse);
        }
        info() << "Transaction will be closed and send ping";
        sendMessage<PingMessage>(
            mContractorID,
            mContractorsManager->idOnContractorSide(mContractorID));
        return resultDone();
    }
    auto message = popNextMessage<TrustLineConfirmationMessage>();
    info() << "contractor " << message->idOnReceiverSide << " send response on opening TL. gateway: " << message->isContractorGateway();
    if (message->idOnReceiverSide != mContractorID) {
        warning() << "Sender is not contractor of this transaction";
        return resultContinuePreviousState();
    }
    if (!mTrustLines->trustLineIsPresent(mContractorID)) {
        error() << "Something wrong, because TL must be created";
        // todo : need correct reaction
        return resultDone();
    }

    auto ioTransaction = mStorageHandler->beginTransaction();
    if (message->state() != ConfirmationMessage::OK) {
        warning() << "Contractor didn't accept opening TL. Response code: " << message->state();
        mTrustLines->removeTrustLine(
            mContractorID,
            ioTransaction);
        return resultDone();
    }

    try {
        mTrustLines->setTrustLineState(
            mContractorID,
            TrustLine::Active,
            ioTransaction);
        if (message->isContractorGateway()) {
            mTrustLines->setContractorAsGateway(
                ioTransaction,
                mContractorID,
                true);
        }

#ifdef TESTS
        mTrustLinesInfluenceController->testThrowExceptionOnSourceProcessingResponseStage(
            BaseTransaction::OpenTrustLineTransaction);
        mTrustLinesInfluenceController->testTerminateProcessOnSourceProcessingResponseStage(
            BaseTransaction::OpenTrustLineTransaction);
#endif

        populateHistory(
            ioTransaction,
            TrustLineRecord::Opening);
    } catch (IOError &e) {
        ioTransaction->rollback();
        error() << "Attempt to process confirmation from contractor " << mContractorID << " failed. "
                << "IO transaction can't be completed. Details are: " << e.what();
        throw e;
    }

    try {
        mEventsInterfaceManager->writeEvent(
            Event::initTrustLineEvent(
                mContractorsManager->selfContractor()->mainAddress(),
                mContractorsManager->contractorMainAddress(mContractorID),
                mEquivalent));
    } catch (std::exception &e) {
        warning() << "Can't write init TL event " << e.what();
    }

    publicKeysSharingSignal(mContractorID, mEquivalent);
    return resultDone();
}

TransactionResult::SharedConst OpenTrustLineTransaction::resultOK()
{
    return transactionResultFromCommandAndWaitForMessageTypes(
               mCommand->responseOK(),
    {Message::TrustLines_Confirmation},
    kWaitMillisecondsForResponse);
}

TransactionResult::SharedConst OpenTrustLineTransaction::resultForbiddenRun()
{
    return transactionResultFromCommand(
               mCommand->responseForbiddenRunTransaction());
}

TransactionResult::SharedConst OpenTrustLineTransaction::resultProtocolError()
{
    return transactionResultFromCommand(
               mCommand->responseProtocolError());
}

TransactionResult::SharedConst OpenTrustLineTransaction::resultTrustLineAlreadyExist()
{
    return transactionResultFromCommand(
               mCommand->responseAlreadyCreated());
}

TransactionResult::SharedConst OpenTrustLineTransaction::resultUnexpectedError()
{
    return transactionResultFromCommand(
               mCommand->responseUnexpectedError());
}

const string OpenTrustLineTransaction::logHeader() const
{
    stringstream s;
    s << "[OpenTrustLineTA: " << currentTransactionUUID() << " " << mEquivalent << "]";
    return s.str();
}

void OpenTrustLineTransaction::populateHistory(
    IOTransaction::Shared ioTransaction,
    TrustLineRecord::TrustLineOperationType operationType)
{
#ifndef TESTS
    auto record = make_shared<TrustLineRecord>(
                      mTransactionUUID,
                      operationType,
                      mContractorsManager->contractor(mContractorID),
                      0);

    ioTransaction->historyStorage()->saveTrustLineRecord(
        record,
        mEquivalent);
#endif
}