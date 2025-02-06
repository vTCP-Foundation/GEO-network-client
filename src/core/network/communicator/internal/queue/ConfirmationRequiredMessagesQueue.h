#ifndef CONFIRMATIONREQUIREDMESSAGESQUEUE_H
#define CONFIRMATIONREQUIREDMESSAGESQUEUE_H

#include "../../../../common/time/TimeUtils.h"
#include "../../../messages/base/transaction/ConfirmationMessage.h"
#include "../../../messages/gateway_notification_and_routing_tables/GatewayNotificationMessage.h"

#include <boost/signals2.hpp>

#include <map>

namespace signals = boost::signals2;

/**
 * Stores messages, that must be re-sent to the remote node,
 * until appropriate confirmation would be received.
 */
class ConfirmationRequiredMessagesQueue
{
public:
    typedef shared_ptr<ConfirmationRequiredMessagesQueue> Shared;

public:
    signals::signal<void(ContractorID, const SerializedEquivalent, Message::SerializedType)> signalRemoveMessageFromStorage;

    signals::signal<void(ContractorID, TransactionMessage::Shared)> signalSaveMessageToStorage;

public:
    ConfirmationRequiredMessagesQueue(
        const SerializedEquivalent equivalent,
        ContractorID contractorID)
    noexcept;

    /**
     * Checks message type, and in case if this message requires confirmation -
     * adds it to the internal queue for further re-sending. Otherwise - does nothing.
     */
    bool enqueue(
        TransactionMessage::Shared message);

    /**
     * Cancels resending of the message with transaction UUID = "transactionUUID".
     * @returns true in case if queue was containing appropriate message, otherwise - returns false.
     */
    bool tryProcessConfirmation(
        ConfirmationMessage::Shared confirmationMessage);

    /**
     * @returns date time when next sending attempt must be scheduled.
     */
    const DateTime &nextSendingAttemptDateTime()
    noexcept;

    /**
     * @returns messages, that are enqueued by this queue.
     * On each call, internal timer of next re-sending is exponentially increased.
     */
    const map<TransactionUUID, TransactionMessage::Shared> &messages()
    noexcept;

    /**
     * @returns messages count in the queue.
     */
    const size_t size() const
    noexcept;

protected:
    /**
     * Sets re-sending timeout to the default value.
     */
    void resetInternalTimeout()
    noexcept;

protected: // messages handlers
    /**
     * Adds "message" to the queue for further re-sending.
     */
    void updateGatewayNotificationInTheQueue(
        TransactionMessage::Shared message);

protected:
    // Stores messages queue by the transactionUUID.
    // TransactionUUID is used as key to be able to remove messages from the queue,
    // when appropriate confirmation would be received.
    //
    // Map is used because current GCC realisation of map is faster than unordered_map,
    // until several thousands of items in the map.
    map<TransactionUUID, TransactionMessage::Shared> mMessages;

    // Stores timeout that must be waited before next sending attempt.
    // This timeout would be exponentially increased on each sending attempt.
    uint16_t mNextTimeoutSeconds;

    // Stores date time, when messages from this queue must be sent to the remote node.
    // On each sending attempt this timeout must be increased by the mNextTimeoutSeconds.
    DateTime mNextSendingAttemptDateTime;

    ContractorID mContractorID;
    SerializedEquivalent mEquivalent;
};

#endif // CONFIRMATIONREQUIREDMESSAGESQUEUE_H
