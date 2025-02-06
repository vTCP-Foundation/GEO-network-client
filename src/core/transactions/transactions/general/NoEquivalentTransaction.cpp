#include "NoEquivalentTransaction.h"

NoEquivalentTransaction::NoEquivalentTransaction(
    BaseUserCommand::Shared command,
    Logger &logger):
    BaseTransaction(
        BaseTransaction::NoEquivalentType,
        0,
        logger),
    mCommand(command),
    mMessage(nullptr)
{}

NoEquivalentTransaction::NoEquivalentTransaction(
    TransactionMessage::Shared message,
    Logger &logger):
    BaseTransaction(
        BaseTransaction::NoEquivalentType,
        0,
        logger),
    mCommand(nullptr),
    mMessage(message)
{}

TransactionResult::SharedConst NoEquivalentTransaction::run()
{
    if (mCommand != nullptr) {
        return transactionResultFromCommand(
                   mCommand->responseEquivalentIsAbsent());
    } else if (mMessage != nullptr) {
        // todo : inspect what type of message address is present in message
        sendMessage<NoEquivalentMessage>(
            mMessage->senderAddresses.at(0),
            mMessage->equivalent(),
            mMessage->transactionUUID());
    } else {
        error() << "Command and Message are empty";
    }
    return resultDone();
}

const string NoEquivalentTransaction::logHeader() const
{
    stringstream s;
    s << "[NoEquivalentTA: " << currentTransactionUUID() << " " << mEquivalent << "]";
    return s.str();
}