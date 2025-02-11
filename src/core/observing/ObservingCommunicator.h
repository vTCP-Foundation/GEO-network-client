#ifndef VTCPD_OBSERVINGCOMMUNICATOR_H
#define VTCPD_OBSERVINGCOMMUNICATOR_H

#include "../contractors/addresses/IPv4WithPortAddress.h"
#include "messages/base/ObservingMessage.hpp"
#include "../logger/Logger.h"

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class ObservingCommunicator
{

public:
    ObservingCommunicator(
        IOCtx &ioCtx,
        Logger &logger);

    BytesShared sendRequestToObserver(
        IPv4WithPortAddress::Shared observerAddress,
        ObservingMessage::Shared request);

protected:
    static string logHeader();

    LoggerStream error() const;

    LoggerStream debug() const;

    LoggerStream info() const;

private:
    IOCtx &mIOCtx;
    Logger &mLogger;
};


#endif //VTCPD_OBSERVINGCOMMUNICATOR_H
