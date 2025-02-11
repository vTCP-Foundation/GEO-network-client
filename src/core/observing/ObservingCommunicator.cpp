#include "ObservingCommunicator.h"

ObservingCommunicator::ObservingCommunicator(
    IOCtx &ioCtx,
    Logger &logger):
    mIOCtx(ioCtx),
    mLogger(logger)
{}

BytesShared ObservingCommunicator::sendRequestToObserver(
    IPv4WithPortAddress::Shared observerAddress,
    ObservingMessage::Shared request)
{
#ifdef DEBUG_LOG_OBSEVING_HANDLER
    debug() << "sendRequestToObserver " << observerAddress->fullAddress();
#endif
    tcp::resolver resolver(mIOCtx);
    boost::system::error_code errorCode;

    auto endpoints = resolver.resolve(
                         observerAddress->host(),
                         to_string(observerAddress->port()),
                         errorCode);

    if (errorCode) {
        throw errorCode;
    }

    tcp::socket socket(mIOCtx);
    boost::asio::connect(socket, endpoints, errorCode);

    if (errorCode) {
        throw errorCode;
    }

    auto serializedRequest = request->serializeToBytes();
    boost::asio::write(
        socket,
        boost::asio::buffer(
            serializedRequest.get(),
            request->serializedSize()));

#ifdef DEBUG_LOG_OBSEVING_HANDLER
    debug() << "request sent";
#endif

    ObservingMessage::MessageSize replySize;
    boost::asio::read(
        socket,
        boost::asio::buffer(
            &replySize,
            sizeof(ObservingMessage::MessageSize)));

    if (replySize > 32 * 1024 * 1024) {
        error() << "Reply size is too large";
        throw boost::asio::error::message_size;
    }
#ifdef DEBUG_LOG_OBSEVING_HANDLER
    debug() << "obtained reply size " << replySize;
#endif

    BytesShared result = tryMalloc(replySize);
    boost::asio::read(
        socket,
        boost::asio::buffer(
            result.get(),
            replySize));

    socket.close();
    return result;
}

string ObservingCommunicator::logHeader()
{
    return "ObservingCommunicator";
}

LoggerStream ObservingCommunicator::error() const
{
    return mLogger.error(logHeader());
}

LoggerStream ObservingCommunicator::debug() const
{
    return mLogger.debug(logHeader());
}

LoggerStream ObservingCommunicator::info() const
{
    return mLogger.info(logHeader());
}