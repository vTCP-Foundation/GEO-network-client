﻿#include "Communicator.h"


Communicator::Communicator(
    IOService &IOService,
    Host host,
    Port port,
    ContractorsManager *contractorsManager,
    TailManager *tailManager,
    ProvidingHandler *providingHandler,
    Logger &logger):

    mIOService(IOService),
    mContractorsManager(contractorsManager),
    mTailManager(tailManager),
    mLog(logger),

    mCommunicatorStorageHandler(
        make_unique<CommunicatorStorageHandler>(
            // todo : move this consts to Core.h
            "io",
            "communicatorStorageDB",
            logger)),

    mConfirmationRequiredMessagesHandler(
        make_unique<ConfirmationRequiredMessagesHandler>(
            IOService,
            mCommunicatorStorageHandler.get(),
            logger)),

    mConfirmationNotStronglyRequiredMessagesHandler(
        make_unique<ConfirmationNotStronglyRequiredMessagesHandler>(
            IOService,
            logger)),

    mConfirmationResponseMessagesHandler(
        make_unique<ConfirmationResponseMessagesHandler>(
            IOService,
            logger)),

    mPingMessagesHandler(
        make_unique<PingMessagesHandler>(
            contractorsManager,
            IOService,
            logger))
{
    if (!host.empty()) {
        mSocket = make_unique<UDPSocket>(
                      IOService,
                      udp::endpoint(
                          address::from_string(host),
                          port));
#ifdef DEBUG_LOG_NETWORK_COMMUNICATOR
        logger.debug("Communicator mSocket ") << host << ":" << port;
#endif
    } else {
        mSocket = make_unique<UDPSocket>(
                      IOService,
                      udp::endpoint(
                          udp::v4(),
                          contractorsManager->selfContractor()->mainAddress()->port()));
#ifdef DEBUG_LOG_NETWORK_COMMUNICATOR
        logger.debug("Communicator mSocket v4()") << ":" << port;
#endif
    }

    mIncomingMessagesHandler =
        make_unique<IncomingMessagesHandler>(
            IOService,
            *mSocket,
            contractorsManager,
            tailManager,
            logger);

    mOutgoingMessagesHandler =
        make_unique<OutgoingMessagesHandler>(
            IOService,
            *mSocket,
            contractorsManager,
            providingHandler,
            logger);

    // Direct signals chaining.
    mIncomingMessagesHandler->signalMessageParsed.connect(
        boost::bind(
            &Communicator::onMessageReceived,
            this,
            _1));

    mConfirmationRequiredMessagesHandler->signalOutgoingMessageReady.connect(
        boost::bind(
            &Communicator::onConfirmationRequiredMessageReadyToResend,
            this,
            _1));

    mConfirmationNotStronglyRequiredMessagesHandler->signalOutgoingMessageReady.connect(
        boost::bind(
            &Communicator::onConfirmationNotStronglyRequiredMessageReadyToResend,
            this,
            _1,
            _2));

    mConfirmationNotStronglyRequiredMessagesHandler->signalClearTopologyCache.connect(
        boost::bind(
            &Communicator::onClearTopologyCache,
            this,
            _1,
            _2));

    mPingMessagesHandler->signalOutgoingMessageReady.connect(
        boost::bind(
            &Communicator::onPingMessageReadyToResend,
            this,
            _1));
}

void Communicator::beginAcceptMessages()
noexcept
{
    mIncomingMessagesHandler->beginReceivingData();
}

/**
 * Tries to send message to the remote node.
 * Because there is no way to know, if the message was delivered - this method doesn't returns anything.
 * Internal transactions logic must take care about the case when message was lost.
 *
 * This method also doesn't reports any internal errors.
 * In case if some internal errors would occur - all of them would be only logged.
 *
 *
 * @param message - message that must be sent to the remote node.
 * @param contractorID - id of the remote node which should receive the message.
 */
void Communicator::sendMessage (
    const Message::Shared message,
    const ContractorID contractorID)
noexcept
{
    // Filter outgoing messages for confirmation-required messages.
    if (message->isAddToConfirmationRequiredMessagesHandler()) {
        mConfirmationRequiredMessagesHandler->tryEnqueueMessage(
            contractorID,
            message);
    }

    if (message->typeID() == Message::General_Ping) {
        mPingMessagesHandler->tryEnqueueContractor(
            contractorID);
    }

    mOutgoingMessagesHandler->sendMessage(
        message,
        contractorID);
}

void Communicator::sendMessage (
    const Message::Shared message,
    const BaseAddress::Shared contractorAddress)
noexcept
{
    if (message->isAddToConfirmationNotStronglyRequiredMessagesHandler()) {
        mConfirmationNotStronglyRequiredMessagesHandler->tryEnqueueMessage(
            contractorAddress,
            message);
    }

    mOutgoingMessagesHandler->sendMessage(
        message,
        contractorAddress);
}

void Communicator::sendMessageWithCacheSaving(
    const TransactionMessage::Shared message,
    ContractorID contractorID,
    Message::MessageType incomingMessageTypeFilter,
    uint32_t cacheLivingTime)
noexcept
{
    mConfirmationResponseMessagesHandler->addCachedMessage(
        contractorID,
        message,
        incomingMessageTypeFilter,
        cacheLivingTime);

    mOutgoingMessagesHandler->sendMessage(
        message,
        contractorID);
}

void Communicator::processConfirmationMessage(
    ConfirmationMessage::Shared confirmationMessage)
{
    mConfirmationRequiredMessagesHandler->tryProcessConfirmation(
        confirmationMessage);
}

void Communicator::processPongMessage(
    ContractorID contractorID)
{
    mPingMessagesHandler->tryProcessPongMessage(
        contractorID);
}

void Communicator::enqueueContractorWithPostponedSending(
    ContractorID contractorID)
{
    mPingMessagesHandler->enqueueContractorWithPostponedSending(
        contractorID);
}

void Communicator::onMessageReceived(
    Message::Shared message)
{
#ifdef DEBUG_LOG_NETWORK_COMMUNICATOR
    debug() << "onMessageReceived " << message->typeID();
#endif
    // these messages contain parts of topology and after theirs receiving we should send confirmation
    if (message->typeID() == Message::MaxFlow_ResultMaxFlowCalculation ||
            message->typeID() == Message::MaxFlow_ResultMaxFlowCalculationFromGateway) {
        const auto kResultMaxFlowCalculationMessage =
            static_pointer_cast<MaxFlowCalculationConfirmationMessage>(message);
        sendMessage(
            make_shared<MaxFlowCalculationConfirmationMessage>(
                kResultMaxFlowCalculationMessage->equivalent(),
                mContractorsManager->ownAddresses(),
                kResultMaxFlowCalculationMessage->confirmationID()),
            kResultMaxFlowCalculationMessage->senderAddresses.at(0));
        return;
    }

    // In case if received message is of type "max flow confirmation message" -
    // then it must not be transferred for further processing.
    // Instead of that, it must be transferred for processing into
    // confirmation not strongly required messages handler.
    else if (message->typeID() == Message::MaxFlow_Confirmation) {
        const auto kConfirmationMessage =
            static_pointer_cast<MaxFlowCalculationConfirmationMessage>(message);
        mConfirmationNotStronglyRequiredMessagesHandler->tryProcessConfirmation(
            kConfirmationMessage);
        return;
    } else if (message->typeID() == Message::ProvidingAddressResponse) {
        mOutgoingMessagesHandler->processProviderResponse(
            static_pointer_cast<ProvidingAddressResponseMessage>(
                message));
        return;

    } else if (message->typeID() == Message::Channel_Init) {
        const auto initMessage = static_pointer_cast<InitChannelMessage>(message);
        auto contractorID = mContractorsManager->contractorIDByAddresses(initMessage->senderAddresses);
        mConfirmationRequiredMessagesHandler->tryProcessConfirmation(make_shared<ConfirmationMessage>(
                    initMessage->equivalent(),
                    contractorID,
                    initMessage->transactionUUID()));

    } else if (message->typeID() == Message::General_Ping) {
        const auto pingMessage =
            static_pointer_cast<PingMessage>(message);
        sendMessage(
            make_shared<PongMessage>(
                mContractorsManager->idOnContractorSide(pingMessage->idOnReceiverSide)),
            pingMessage->idOnReceiverSide);
        return;

        // In case if received message is of type "confirmation message" -
        // then it must not be transferred for further processing.
        // Instead of that, it must be transferred for processing into
        // confirmation required messages handler.
    } else if (message->typeID() == Message::System_Confirmation) {
        mConfirmationRequiredMessagesHandler->tryProcessConfirmation(
            static_pointer_cast<ConfirmationMessage>(message));
        return;

    } else if (message->typeID() == Message::RoutingTableResponse) {
        const auto kConfirmationMessage =
            static_pointer_cast<ConfirmationMessage>(message);
        mConfirmationRequiredMessagesHandler->tryProcessConfirmation(
            kConfirmationMessage);
        return;
    }

    if (message->isCheckCachedResponse()) {
        auto incomingTransactionMessage = static_pointer_cast<TransactionMessage>(message);
        auto cachedResponse = mConfirmationResponseMessagesHandler->getCachedMessage(incomingTransactionMessage);
        if (cachedResponse != nullptr) {
            sendMessage(
                cachedResponse,
                incomingTransactionMessage->idOnReceiverSide);
#ifdef DEBUG_LOG_NETWORK_COMMUNICATOR
            debug() << "send cached response";
#endif
            return;
        }
    }

    signalMessageReceived(message);
}

void Communicator::onConfirmationRequiredMessageReadyToResend(
    pair<ContractorID, TransactionMessage::Shared> addresseeAndMessage)
{
    mOutgoingMessagesHandler->sendMessage(
        addresseeAndMessage.second,
        addresseeAndMessage.first);
}

void Communicator::onConfirmationNotStronglyRequiredMessageReadyToResend(
    BaseAddress::Shared contractorAddress,
    MaxFlowCalculationConfirmationMessage::Shared message)
{
    mOutgoingMessagesHandler->sendMessage(
        message,
        contractorAddress);
}

void Communicator::onPingMessageReadyToResend(
    pair<ContractorID, PingMessage::Shared> addresseeAndMessage)
{
    mOutgoingMessagesHandler->sendMessage(
        addresseeAndMessage.second,
        addresseeAndMessage.first);
}

void Communicator::onClearTopologyCache(
    const SerializedEquivalent equivalent,
    BaseAddress::Shared nodeAddress)
{
    signalClearTopologyCache(
        equivalent,
        nodeAddress);
}

string Communicator::logHeader()
noexcept
{
    return "[Communicator]";
}

LoggerStream Communicator::info() const
noexcept
{
    return mLog.info(logHeader());
}

LoggerStream Communicator::debug() const
noexcept
{
    return mLog.debug(logHeader());
}

LoggerStream Communicator::error() const
noexcept
{
    return mLog.error(logHeader());
}
