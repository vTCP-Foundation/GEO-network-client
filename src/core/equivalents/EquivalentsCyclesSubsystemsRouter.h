#ifndef VTCPD_EQUIVALENTSCYCLESSUBSYSTEMSROUTER_H
#define VTCPD_EQUIVALENTSCYCLESSUBSYSTEMSROUTER_H

#include "../cycles/CyclesManager.h"
#include "../cycles/RoutingTableManager.h"
#include "../transactions/scheduler/TransactionsScheduler.h"
#include "../subsystems_controller/SubsystemsController.h"

#include <map>

namespace as = boost::asio;
namespace signals = boost::signals2;

class EquivalentsCyclesSubsystemsRouter
{

public:
    typedef signals::signal<void(const SerializedEquivalent equivalent)> BuildSixNodesCyclesSignal;
    typedef signals::signal<void(const SerializedEquivalent equivalent)> BuildFiveNodesCyclesSignal;
    typedef signals::signal<void(
        const SerializedEquivalent equivalent,
        Path::Shared cycle)> CloseCycleSignal;

public:
    EquivalentsCyclesSubsystemsRouter(
        TransactionsScheduler *transactionScheduler,
        SubsystemsController *subsystemsController,
        as::io_context &ioCtx,
        vector<SerializedEquivalent> equivalents,
        Logger &logger);

    CyclesManager* cyclesManager(
        const SerializedEquivalent equivalent) const;

    RoutingTableManager* routingTableManager(
        const SerializedEquivalent equivalent) const;

    void initNewEquivalent(
        const SerializedEquivalent equivalent);

    void clearRoutingTables();

public:
    mutable CloseCycleSignal closeCycleSignal;

    mutable BuildSixNodesCyclesSignal buildSixNodesCyclesSignal;

    mutable BuildFiveNodesCyclesSignal buildFiveNodesCyclesSignal;

protected:
    string logHeader() const;

    LoggerStream warning() const;

    LoggerStream error() const;

    LoggerStream info() const;

    LoggerStream debug() const;

private:
    void connectSignalsToSlots();

    void subscribeForBuildingFiveNodesCycles(
        CyclesManager::BuildFiveNodesCyclesSignal &signal);

    void subscribeForBuildingSixNodesCycles(
        CyclesManager::BuildSixNodesCyclesSignal &signal);

    void subscribeForClosingCycles(
        CyclesManager::CloseCycleSignal &signal);

    void onBuildCycleFiveNodesSlot(
        const SerializedEquivalent equivalent);

    void onBuildCycleSixNodesSlot(
        const SerializedEquivalent equivalent);

    void onCloseCycleSlot(
        const SerializedEquivalent equivalent,
        Path::Shared cycle);

private:
    as::io_context &mIOCtx;
    TransactionsScheduler *mTransactionScheduler;
    SubsystemsController *mSubsystemsController;
    Logger &mLogger;
    map<SerializedEquivalent, unique_ptr<CyclesManager>> mCyclesManagers;
    map<SerializedEquivalent, unique_ptr<RoutingTableManager>> mRoutingTablesManagers;
};


#endif //VTCPD_EQUIVALENTSCYCLESSUBSYSTEMSROUTER_H
