#include "AuditSourceTransaction.h"

AuditSourceTransaction::AuditSourceTransaction(
    ContractorID contractorID,
    const SerializedEquivalent equivalent,
    ContractorsManager *contractorsManager,
    TrustLinesManager *manager,
    StorageHandler *storageHandler,
    Keystore *keystore,
    FeaturesManager *featuresManager,
    TrustLinesInfluenceController *trustLinesInfluenceController,
    Logger &logger) :
    BaseTrustLineTransaction(
        BaseTransaction::AuditSourceTransactionType,
        equivalent,
        contractorID,
        contractorsManager,
        manager,
        storageHandler,
        keystore,
        featuresManager,
        trustLinesInfluenceController,
        logger),
    mCountSendingAttempts(0)
{
    mAuditNumber = mTrustLines->auditNumber(mContractorID) + 1;
    mStep = Initialization;
}

AuditSourceTransaction::AuditSourceTransaction(
    const SerializedEquivalent equivalent,
    ContractorsManager *contractorsManager,
    ContractorID contractorID,
    TrustLinesManager *manager,
    StorageHandler *storageHandler,
    Keystore *keystore,
    FeaturesManager *featuresManager,
    TrustLinesInfluenceController *trustLinesInfluenceController,
    Logger &logger) :
    BaseTrustLineTransaction(
        BaseTransaction::AuditSourceTransactionType,
        equivalent,
        contractorID,
        contractorsManager,
        manager,
        storageHandler,
        keystore,
        featuresManager,
        trustLinesInfluenceController,
        logger),
    mCountSendingAttempts(0)
{
    mAuditNumber = mTrustLines->auditNumber(mContractorID);
    mStep = NextAttempt;
}

TransactionResult::SharedConst AuditSourceTransaction::run()
{
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

TransactionResult::SharedConst AuditSourceTransaction::runInitializationStage()
{
    info() << "runInitializationStage " << mContractorID;

    if (!mContractorsManager->contractorPresent(mContractorID)) {
        warning() << "There is no contractor with requested id";
        return resultDone();
    }
    info() << "Contractor present";

    try {
        if (mTrustLines->trustLineState(mContractorID) != TrustLine::Active) {
            warning() << "Invalid TL state " << mTrustLines->trustLineState(mContractorID);
            return resultDone();
        }
        info() << "TL active";

    } catch (NotFoundError &e) {
        warning() << "Attempt to audit not existing TL";
        return resultDone();
    }

    // todo maybe check in storage (keyChain)
    if (!mTrustLines->trustLineOwnKeysPresent(mContractorID)) {
        warning() << "There are no own keys";
        return resultDone();
    }
    info() << "Own keys present";

    // todo maybe check in storage (keyChain)
    if (!mTrustLines->trustLineContractorKeysPresent(mContractorID)) {
        warning() << "There are no contractor keys";
        return resultDone();
    }
    info() << "Contractor keys present";

    mTrustLines->setTrustLineState(
        mContractorID,
        TrustLine::AuditPending);
    info() << "TL set as AuditPending";

    // note: io transaction would commit automatically on destructor call.
    // there is no need to call commit manually.
    auto ioTransaction = mStorageHandler->beginTransaction();
    auto keyChain = mKeysStore->keychain(
        mTrustLines->trustLineID(mContractorID));
    info() << "KeyChain taken";
    try {
        auto ownPublicKeysHash = keyChain.ownPublicKeysHash(ioTransaction);
        info() << "ownPublicKeysHash " << ownPublicKeysHash->toString();
        auto contractorPublicKeysHash = keyChain.contractorPublicKeysHash(ioTransaction);
        info() << "contractorPublicKeysHash " << contractorPublicKeysHash->toString();
        auto serializedAuditData = getOwnSerializedAuditData(
            ownPublicKeysHash,
            contractorPublicKeysHash);
        mOwnSignatureAndKeyNumber = keyChain.sign(
            ioTransaction,
            serializedAuditData.first,
            serializedAuditData.second);

        keyChain.saveOwnAuditPart(
            ioTransaction,
            mAuditNumber,
            mOwnSignatureAndKeyNumber.second,
            mOwnSignatureAndKeyNumber.first,
            ownPublicKeysHash,
            contractorPublicKeysHash,
            mTrustLines->incomingTrustAmount(
                mContractorID),
            mTrustLines->outgoingTrustAmount(
                mContractorID),
            mTrustLines->balance(
                mContractorID));

        mTrustLines->setTrustLineAuditNumber(
            mContractorID,
            mAuditNumber);

#ifdef TESTS
        mTrustLinesInfluenceController->testThrowExceptionOnSourceInitializationStage(
            BaseTransaction::AuditSourceTransactionType);
        mTrustLinesInfluenceController->testTerminateProcessOnTargetStage(
            BaseTransaction::AuditSourceTransactionType);
#endif

    } catch(IOError &e) {
        ioTransaction->rollback();
        mTrustLines->setTrustLineState(
            mContractorID,
            TrustLine::Active);
        warning() << "Attempt to audit trust line to the node " << mContractorID << " failed. "
                  << "Can't sign audit data. IO transaction can't be completed. "
                  << "Details are: " << e.what();

        // Rethrowing the exception,
        // because the TA can't finish properly and no result may be returned.
        throw e;
    }

    // Notifying remote node about trust line state changed.
    sendMessage<AuditMessage>(
        mContractorID,
        mEquivalent,
        mContractorsManager->contractor(mContractorID),
        mTransactionUUID,
        mAuditNumber,
        mTrustLines->incomingTrustAmount(mContractorID),
        mTrustLines->outgoingTrustAmount(mContractorID),
        mOwnSignatureAndKeyNumber.second,
        mOwnSignatureAndKeyNumber.first);
    mCountSendingAttempts++;
    info() << "Send audit message signed by key " << mOwnSignatureAndKeyNumber.second;

    mStep = ResponseProcessing;
    return resultWaitForMessageTypes(
        {Message::TrustLines_AuditConfirmation},
        kWaitMillisecondsForResponse);
}

TransactionResult::SharedConst AuditSourceTransaction::runNextAttemptStage()
{
    info() << "runNextAttemptStage " << mContractorID;

    if (!mContractorsManager->contractorPresent(mContractorID)) {
        warning() << "There is no contractor with requested id";
        return resultDone();
    }

    try {
        if (mTrustLines->trustLineState(mContractorID) != TrustLine::AuditPending) {
            warning() << "Invalid TL state " << mTrustLines->trustLineState(mContractorID);
            return resultDone();
        }

    } catch (NotFoundError &e) {
        warning() << "Attempt to audit not existing TL";
        return resultDone();
    }

    processPongMessage(mContractorID);

    // todo maybe check in storage (keyChain)
    if (!mTrustLines->trustLineOwnKeysPresent(mContractorID)) {
        warning() << "There are no own keys";
        return resultDone();
    }

    // todo maybe check in storage (keyChain)
    if (!mTrustLines->trustLineContractorKeysPresent(mContractorID)) {
        warning() << "There are no contractor keys";
        return resultDone();
    }

    // note: io transaction would commit automatically on destructor call.
    // there is no need to call commit manually.
    auto ioTransaction = mStorageHandler->beginTransaction();
    auto keyChain = mKeysStore->keychain(
            mTrustLines->trustLineID(mContractorID));
    try {
        mOwnSignatureAndKeyNumber = keyChain.getSignatureAndKeyNumberForPendingAudit(
            ioTransaction,
            mAuditNumber);
        debug() << "signature getting";

#ifdef TESTS
        mTrustLinesInfluenceController->testThrowExceptionOnSourceResumingStage(
            BaseTransaction::AuditSourceTransactionType);
        mTrustLinesInfluenceController->testTerminateProcessOnSourceResumingStage(
            BaseTransaction::AuditSourceTransactionType);
#endif

    } catch(IOError &e) {
        ioTransaction->rollback();
        warning() << "Attempt to audit trust line to the node " << mContractorID << " failed. "
                  << "Can't get audit data. IO transaction can't be completed. "
                  << "Details are: " << e.what();

        // Rethrowing the exception,
        // because the TA can't finish properly and no result may be returned.
        throw e;
    }

    // Notifying remote node about trust line state changed.
    sendMessage<AuditMessage>(
        mContractorID,
        mEquivalent,
        mContractorsManager->contractor(mContractorID),
        mTransactionUUID,
        mAuditNumber,
        mTrustLines->incomingTrustAmount(mContractorID),
        mTrustLines->outgoingTrustAmount(mContractorID),
        mOwnSignatureAndKeyNumber.second,
        mOwnSignatureAndKeyNumber.first);
    mCountSendingAttempts++;
    info() << "Send audit message signed by key " << mOwnSignatureAndKeyNumber.second;

    mStep = ResponseProcessing;
    return resultWaitForMessageTypes(
        {Message::TrustLines_AuditConfirmation},
        kWaitMillisecondsForResponse);
}

TransactionResult::SharedConst AuditSourceTransaction::runResponseProcessingStage()
{
    info() << "runResponseProcessingStage";
    if (mContext.empty()) {
        warning() << "Contractor don't send response.";
        if (mCountSendingAttempts < kMaxCountSendingAttempts) {
            sendMessage<AuditMessage>(
                mContractorID,
                mEquivalent,
                mContractorsManager->contractor(mContractorID),
                mTransactionUUID,
                mAuditNumber,
                mTrustLines->incomingTrustAmount(mContractorID),
                mTrustLines->outgoingTrustAmount(mContractorID),
                mOwnSignatureAndKeyNumber.second,
                mOwnSignatureAndKeyNumber.first);
            mCountSendingAttempts++;
            info() << "Send message " << mCountSendingAttempts << " times";
            return resultWaitForMessageTypes(
                {Message::TrustLines_AuditConfirmation},
                kWaitMillisecondsForResponse);
        }
        info() << "Transaction will be closed and send ping";
        sendMessage<PingMessage>(
            mContractorID,
            mContractorsManager->idOnContractorSide(mContractorID));
        return resultDone();
    }

    auto message = popNextMessage<AuditResponseMessage>();
    info() << "contractor " << message->idOnReceiverSide << " confirmed audit.";
    if (message->idOnReceiverSide != mContractorID) {
        warning() << "Sender is not contractor of this transaction";
        return resultContinuePreviousState();
    }

    if (!mTrustLines->trustLineIsPresent(mContractorID)) {
        warning() << "Something wrong, because TL must be created";
        // todo : need correct reaction
        return resultDone();
    }

    if (message->state() == ConfirmationMessage::ReservationsPresentOnTrustLine) {
        info() << "Contractor's TL is not ready for audit yet";
        // message on communicator queue, wait for audit response after reservations committing or cancelling
        // todo add timeout or count failed attempts for running conflict resolver TA
        return resultWaitForMessageTypes(
            {Message::TrustLines_AuditConfirmation},
            kWaitMillisecondsForResponse);
    }

    auto ioTransaction = mStorageHandler->beginTransaction();
    auto keyChain = mKeysStore->keychain(
        mTrustLines->trustLineID(mContractorID));
    try {

        // todo process ConfirmationMessage::OwnKeysAbsent and ConfirmationMessage::ContractorKeysAbsent

        if (message->state() != ConfirmationMessage::OK) {
            warning() << "Contractor didn't accept changing TL. Response code: " << message->state();
            mTrustLines->setTrustLineState(
                mContractorID,
                TrustLine::ConflictResolving,
                ioTransaction);
            // todo run conflict resolving TA
            return resultDone();
        }

        auto contractorSerializedAuditData = getContractorSerializedAuditData(
            keyChain.ownPublicKeysHash(ioTransaction),
            keyChain.contractorPublicKeysHash(ioTransaction));

        if (!keyChain.checkSign(
                ioTransaction,
                contractorSerializedAuditData.first,
                contractorSerializedAuditData.second,
                message->signature(),
                message->keyNumber())) {
            warning() << "Contractor didn't sign message correct by key number " << message->keyNumber();
            mTrustLines->setTrustLineState(
                mContractorID,
                TrustLine::ConflictResolving,
                ioTransaction);
            // todo run conflict resolver TA
            return resultDone();
        }
        info() << "Contractor sign audit correct";

        keyChain.saveContractorAuditPart(
            ioTransaction,
            mAuditNumber,
            message->keyNumber(),
            message->signature());

        mTrustLines->resetTrustLineTotalReceiptsAmounts(
            mContractorID);
        if (mTrustLines->isTrustLineEmpty(mContractorID) and
                !keyChain.isInitialAuditCondition(ioTransaction)) {
            mTrustLines->setTrustLineState(
                mContractorID,
                TrustLine::Archived,
                ioTransaction);
            keyChain.removeUnusedOwnKeys(ioTransaction);
            mTrustLines->setIsOwnKeysPresent(mContractorID, false);
            keyChain.removeUnusedContractorKeys(ioTransaction);
            mTrustLines->setIsContractorKeysPresent(mContractorID, false);
            info() << "Trust Line become empty";
        } else {
            mTrustLines->setTrustLineState(
                mContractorID,
                TrustLine::Active);
            info() << "All data saved. Now TL is ready for using";
        }

#ifdef TESTS
        mTrustLinesInfluenceController->testThrowExceptionOnSourceProcessingResponseStage(
            BaseTransaction::AuditSourceTransactionType);
        mTrustLinesInfluenceController->testTerminateProcessOnSourceProcessingResponseStage(
            BaseTransaction::AuditSourceTransactionType);
#endif

    } catch (ValueError &e) {
        ioTransaction->rollback();
        // todo need correct reaction, maybe conflict resolver
        error() << "Attempt to save audit from contractor " << mContractorID << " failed. "
                << "Details are: " << e.what();
        return resultDone();
    } catch (IOError &e) {
        ioTransaction->rollback();
        // todo need correct reaction, maybe conflict resolver
        error() << "Attempt to process confirmation from contractor " << mContractorID << " failed. "
                << "IO transaction can't be completed. Details are: " << e.what();
        throw e;
    }

    mTrustLines->resetAuditRule(mContractorID);
    trustLineActionSignal(
        mContractorID,
        mEquivalent,
        false);

    return resultDone();
}

const string AuditSourceTransaction::logHeader() const
{
    stringstream s;
    s << "[AuditSourceTA: " << currentTransactionUUID() << " " << mEquivalent << "]";
    return s.str();
}