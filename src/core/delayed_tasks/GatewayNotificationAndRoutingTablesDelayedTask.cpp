#include "GatewayNotificationAndRoutingTablesDelayedTask.h"

GatewayNotificationAndRoutingTablesDelayedTask::GatewayNotificationAndRoutingTablesDelayedTask(
    as::io_context &ioCtx,
    Logger &logger):

    mIOCtx(ioCtx),
    mLog(logger)
{
    mNotificationTimer = make_unique<as::steady_timer>(
                             mIOCtx);
    // todo : rand() used for concurrent start of all nodes or some part of nodes (data center)
    // on decentralize network it is not necessary
    srand(randomInitializer());
    int timeStarted = 120 + rand() % (600);
    info() << "timeStarted " << timeStarted;
#ifdef TESTS
    timeStarted = 10;
#endif
    mNotificationTimer->expires_after(
        chrono::seconds(
            timeStarted));
    mNotificationTimer->async_wait(
        boost::bind(
            &GatewayNotificationAndRoutingTablesDelayedTask::runSignalNotify,
            this,
            as::placeholders::error));
}

void GatewayNotificationAndRoutingTablesDelayedTask::runSignalNotify(
    const boost::system::error_code &errorCode)
{
    if (errorCode) {
        warning() << errorCode.message().c_str();
    }
    info() << "run gateway notification signal";
    mNotificationTimer->cancel();
    mNotificationTimer->expires_after(
        std::chrono::seconds(
            kUpdatingTimerPeriodSeconds + rand() % (60 * 60 * 24)));
#ifdef TESTS
    mNotificationTimer->cancel();
    mNotificationTimer->expires_after(
        std::chrono::seconds(15));
#endif
    mNotificationTimer->async_wait(
        boost::bind(
            &GatewayNotificationAndRoutingTablesDelayedTask::runSignalNotify,
            this,
            as::placeholders::error));
    gatewayNotificationSignal();
}

uint32_t GatewayNotificationAndRoutingTablesDelayedTask::randomInitializer() const
{
    NodeUUID randomInitializator;
    uint32_t result = 0;
    for (int i=0; i < NodeUUID::kBytesSize; i++) {
        result = result << 2;
        result |= (randomInitializator.data[i] & 0x3);
    }
    return result;
}


LoggerStream GatewayNotificationAndRoutingTablesDelayedTask::debug() const
{
    return mLog.debug(logHeader());
}

LoggerStream GatewayNotificationAndRoutingTablesDelayedTask::info() const
{
    return mLog.info(logHeader());
}

LoggerStream GatewayNotificationAndRoutingTablesDelayedTask::warning() const
{
    return mLog.warning(logHeader());
}

const string GatewayNotificationAndRoutingTablesDelayedTask::logHeader() const
{
    stringstream s;
    s << "GatewayNotificationAndRoutingTablesDelayedTask";
    return s.str();
}