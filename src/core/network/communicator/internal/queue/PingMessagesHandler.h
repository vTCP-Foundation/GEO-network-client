#ifndef VTCPD_PINGMESSAGESHANDLER_H
#define VTCPD_PINGMESSAGESHANDLER_H

#include "../../internal/common/Types.h"
#include "../../../../common/exceptions/RuntimeError.h"
#include "../../../../logger/LoggerMixin.hpp"
#include "../../../../contractors/ContractorsManager.h"
#include "../../../messages/general/PingMessage.h"
#include "../../../messages/general/PongMessage.h"

#include <boost/asio/steady_timer.hpp>
#include <boost/signals2.hpp>

#include <vector>


using namespace std;
namespace as = boost::asio;

class PingMessagesHandler : protected LoggerMixin
{

public:
    /**
     * This signal emits every time, when ends timeout of some queue,
     * and messages of this queue must be sent to the remote node once more time.
     */
    signals::signal<void(pair<ContractorID, PingMessage::Shared>)> signalOutgoingMessageReady;

public:
    PingMessagesHandler(
        ContractorsManager *contractorsManager,
        IOCtx &ioCtx,
        Logger &logger);

    /**
     * @param contractorID - remote node ID.
     */
    void tryEnqueueContractor(
        ContractorID contractorID);

    void enqueueContractorWithPostponedSending(
        ContractorID contractorID);

    /**
     * @param contractorUUID - UUID of the remote node, that sent pong message.
     */
    void tryProcessPongMessage(
        ContractorID contractorID);

protected:
    const string logHeader() const
    noexcept;

    /**
     * Schedules timer for next awakeness.
     */
    void rescheduleResending();

    /**
     * Sends postponed messages to the remote nodes.
     * This method would be called every time when some queue timeout would fire up.
     */
    void sendPingMessages() const;

    void delayedRescheduleResending();

protected:
    static const uint16_t kMessagesReschedulingSecondsTime = 150;
    static const uint16_t kPingMessagesSecondsTimeOut = 60;

private:
    ContractorsManager *mContractorsManager;
    vector<ContractorID> mContractors;

    IOCtx &mIOCtx;
    as::steady_timer mResendingTimer;
    unique_ptr<as::steady_timer> mReschedulingTimer;
};


#endif //VTCPD_PINGMESSAGESHANDLER_H
