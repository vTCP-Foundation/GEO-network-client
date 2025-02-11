#ifndef VTCPD_GATEWAYNOTIFICATIONANDROUTINGTABLESDELAYEDTASK_H
#define VTCPD_GATEWAYNOTIFICATIONANDROUTINGTABLESDELAYEDTASK_H

#include "../common/time/TimeUtils.h"
#include "../logger/Logger.h"
#include "../transactions/transactions/base/TransactionUUID.h"

#include <boost/asio/steady_timer.hpp>
#include <boost/signals2.hpp>
#include <boost/asio.hpp>

using namespace std;
namespace as = boost::asio;
namespace signals = boost::signals2;

class GatewayNotificationAndRoutingTablesDelayedTask
{

public:
    typedef signals::signal<void()> GatewayNotificationSignal;

public:
    GatewayNotificationAndRoutingTablesDelayedTask(
        as::io_context &ioCtx,
        Logger &logger);

public:
    mutable GatewayNotificationSignal gatewayNotificationSignal;

private:
    void runSignalNotify(
        const boost::system::error_code &error);

    uint32_t randomInitializer() const;

    LoggerStream info() const;

    LoggerStream debug() const;

    LoggerStream warning() const;

    const string logHeader() const;

private:
    static const uint32_t kUpdatingTimerPeriodSeconds = 60 * 60 * 24 * 2;

private:
    as::io_context &mIOCtx;
    unique_ptr<as::steady_timer> mNotificationTimer;
    Logger &mLog;
};


#endif //VTCPD_GATEWAYNOTIFICATIONANDROUTINGTABLESDELAYEDTASK_H
