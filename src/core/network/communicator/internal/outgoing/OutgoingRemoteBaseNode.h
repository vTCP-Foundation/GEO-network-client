#ifndef VTCPD_OUTGOINGREMOTEBASENODE_H
#define VTCPD_OUTGOINGREMOTEBASENODE_H

#include "../common/Types.h"
#include "../common/Packet.hpp"

#include "../../../messages/Message.hpp"

#include "../../../../common/memory/MemoryUtils.h"
#include "../../../../logger/Logger.h"
#include "../../../../common/exceptions/Exception.h"

#include <boost/crc.hpp>
#include <boost/asio/steady_timer.hpp>
#include <queue>

namespace as = boost::asio;

class OutgoingRemoteBaseNode
{
public:
    typedef unique_ptr<OutgoingRemoteBaseNode> Unique;

public:
    OutgoingRemoteBaseNode(
        UDPSocket &socket,
        IOCtx &ioCtx,
        IPv4WithPortAddress::Shared remoteAddress,
        Logger &logger);

    virtual ~OutgoingRemoteBaseNode();

    void sendMessage(
        pair<BytesShared, size_t>) noexcept;

    bool containsPacketsInQueue() const;

protected:
    uint32_t crc32Checksum(
        byte_t* data,
        size_t bytesCount)
    const noexcept;

    void populateQueueWithNewPackets(
        byte_t* messageData,
        const size_t bytesCount);

    PacketHeader::ChannelIndex nextChannelIndex() noexcept;

    void beginPacketsSending();

    LoggerStream errors() const;

    LoggerStream debug() const;

protected:
    IOCtx &mIOCtx;
    UDPSocket &mSocket;
    Logger &mLog;

    IPv4WithPortAddress::Shared mRemoteAddress;
    queue<pair<byte_t*, Packet::Size>> mPacketsQueue;
    PacketHeader::ChannelIndex mNextAvailableChannelIndex;
    pair<TimePoint, size_t> mCyclesStats;
    as::steady_timer mSendingDelayTimer;
};

#endif // VTCPD_OUTGOINGREMOTEBASENODE_H
