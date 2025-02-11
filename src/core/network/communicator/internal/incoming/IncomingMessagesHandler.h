﻿#ifndef VTCPD_INCOMINGCONNECTIONSHANDLER_H
#define VTCPD_INCOMINGCONNECTIONSHANDLER_H

#include "../common/Types.h"
#include "../../internal/incoming/IncomingNodesHandler.h"
#include "../../../../common/exceptions/ValueError.h"
#include "../../../../common/exceptions/ConflictError.h"

#include <boost/asio/steady_timer.hpp>
#include <boost/array.hpp>

using namespace std;

class IncomingMessagesHandler
{
public:
    signals::signal<void(Message::Shared)> signalMessageParsed;

public:
    IncomingMessagesHandler(
        IOCtx &ioCtx,
        UDPSocket &socket,
        ContractorsManager *contractorsManager,
        TailManager *tailManager,
        Logger &logger) noexcept;

    void beginReceivingData() noexcept;

protected:
    void handleReceivedInfo(
        const boost::system::error_code &error,
        size_t bytesTransferred) noexcept;

    void rescheduleCleaning() noexcept;

    static string logHeader() noexcept;

    LoggerStream info() const
    noexcept;

    LoggerStream error() const
    noexcept;

    LoggerStream warning() const
    noexcept;

    LoggerStream debug() const
    noexcept;

protected:
    static constexpr const size_t kMaxIncomingBufferSize = Packet::kMaxSize * 2;

protected:
    UDPSocket &mSocket;
    IOCtx &mIOCtx;
    TailManager *mTailManager;
    Logger &mLog;

    boost::array<byte_t, kMaxIncomingBufferSize> mIncomingBuffer;
    UDPEndpoint mRemoteEndpointBuffer;

    MessagesParser mMessagesParser;
    IncomingNodesHandler mRemoteNodesHandler;

    boost::asio::deadline_timer mCleaningTimer;
};

#endif // VTCPD_INCOMINGCONNECTIONSHANDLER_H
