#ifndef GEO_NETWORK_CLIENT_OUTGOINGMESSAGESHANDLER_H
#define GEO_NETWORK_CLIENT_OUTGOINGMESSAGESHANDLER_H

#include "OutgoingNodesHandler.h"
#include "../../../../providing/ProvidingHandler.h"
#include "../../../../contractors/ContractorsManager.h"
#include "../../../messages/providing/ProvidingAddressResponseMessage.h"

#include "../common/Types.h"

class OutgoingMessagesHandler
{
public:
    OutgoingMessagesHandler(
        IOService &ioService,
        UDPSocket &socket,
        ContractorsManager *contractorsManager,
        ProvidingHandler *providingHandler,
        Logger &log) noexcept;

    void sendMessage(
        const Message::Shared message,
        const ContractorID addressee);

    void sendMessage(
        const Message::Shared message,
        const BaseAddress::Shared address);

    void processProviderResponse(
        ProvidingAddressResponseMessage::Shared providerResponse);

private:
    void onPingMessageToProviderReady(
        Provider::Shared provider);

    MsgEncryptor::Buffer pingMessage(
        Provider::Shared provider) const;

    MsgEncryptor::Buffer getRemoteNodeAddressMessage(
        Provider::Shared provider,
        GNSAddress::Shared gnsAddress) const;

    pair<GNSAddress::Shared, IPv4WithPortAddress::Shared> deserializeProviderResponse(
        BytesShared buffer);

    void clearUndeliveredMessages(
        const boost::system::error_code &errorCode);

private:
    static const uint16_t kPostponedMessagesClearingPeriodSeconds = 30;

    static const byte_t kPostponedMessageTimeLiveHours = 0;
    static const byte_t kPostponedMessageTimeLiveMinutes = 0;
    static const byte_t kPostponedMessageTimeLiveSeconds = 10;
    static Duration &kPostponedMessageTimeLiveDuration()
    {
        static auto duration = Duration(
                                   kPostponedMessageTimeLiveHours,
                                   kPostponedMessageTimeLiveMinutes,
                                   kPostponedMessageTimeLiveSeconds);
        return duration;
    }

protected:
    Logger &mLog;
    OutgoingNodesHandler mNodes;
    ContractorsManager *mContractorsManager;
    ProvidingHandler *mProvidingHandler;

    as::steady_timer mPostponedMessagesCleaningTimer;
    multimap<string, pair<MsgEncryptor::Buffer, DateTime>> mPostponedMessages;
};

#endif // GEO_NETWORK_CLIENT_OUTGOINGMESSAGESHANDLER_H
