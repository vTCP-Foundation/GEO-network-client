#ifndef VTCPD_TOPOLOGYCACHEUPDATEDELAYEDTASK_H
#define VTCPD_TOPOLOGYCACHEUPDATEDELAYEDTASK_H

#include "../topology/cache/TopologyCacheManager.h"
#include "../topology/manager/TopologyTrustLinesManager.h"
#include "../topology/cache/MaxFlowCacheManager.h"

#include <boost/asio/steady_timer.hpp>
#include <boost/asio.hpp>

using namespace std;

namespace as = boost::asio;

class TopologyCacheUpdateDelayedTask
{

public:
    TopologyCacheUpdateDelayedTask(
        const SerializedEquivalent equivalent,
        as::io_service &ioService,
        TopologyCacheManager *topologyCacheManager,
        TopologyTrustLinesManager *topologyTrustLineManager,
        MaxFlowCacheManager *maxFlowCalculationNodeCacheManager,
        Logger &logger);

public:
    void runSignalTopologyCacheUpdate(
        const boost::system::error_code &error);

private:
    DateTime minimalAwakeningTimestamp();

    // return time when task should be started again
    DateTime updateCache();

    LoggerStream info() const;

    LoggerStream debug() const;

    LoggerStream warning() const;

    const string logHeader() const;

private:
    static const uint16_t kProlongationTrustLineUpdatingTimeSec = 2;

private:
    static Duration& kProlongationTrustLineUpdatingDuration()
    {
        static auto duration = Duration(
                                   0,
                                   0,
                                   kProlongationTrustLineUpdatingTimeSec);
        return duration;
    }

private:
    as::io_service &mIOService;
    unique_ptr<as::steady_timer> mTopologyCacheUpdateTimer;
    TopologyCacheManager *mTopologyCacheManager;
    TopologyTrustLinesManager *mTopologyTrustLineManager;
    MaxFlowCacheManager *mMaxFlowCalculationNodeCacheManager;
    SerializedEquivalent mEquivalent;
    Logger &mLog;
};


#endif //VTCPD_TOPOLOGYCACHEUPDATEDELAYEDTASK_H
