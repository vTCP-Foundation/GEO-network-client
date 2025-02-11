#include "TopologyEventDelayedTask.h"

TopologyEventDelayedTask::TopologyEventDelayedTask(
    as::io_context &ioCtx,
    EquivalentsSubsystemsRouter *equivalentsSubsystemsRouter,
    Logger &logger) :
    mIOCtx(ioCtx),
    mEquivalentsSubsystemsRouter(equivalentsSubsystemsRouter),
    mLog(logger)
{
    mTopologyEventTimer = make_unique<as::steady_timer>(
                              mIOCtx);

    mTopologyEventTimer->expires_after(
        chrono::seconds(
            +kDelayedTaskTimeSec));
    mTopologyEventTimer->async_wait(boost::bind(
                                        &TopologyEventDelayedTask::runTopologyEvent,
                                        this));
}

void TopologyEventDelayedTask::runTopologyEvent()
{
    mTopologyEventTimer->cancel();
    mEquivalentsSubsystemsRouter->sendTopologyEvent();
}