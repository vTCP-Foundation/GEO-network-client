#include "CyclesManager.h"

CyclesManager::CyclesManager(
    const SerializedEquivalent equivalent,
    TransactionsScheduler *transactionsScheduler,
    as::io_context &ioCtx,
    Logger &logger,
    SubsystemsController *subsystemsController) :

    mEquivalent(equivalent),
    mTransactionScheduler(transactionsScheduler),
    mIOCtx(ioCtx),
    mLog(logger),
    mSubsystemsController(subsystemsController),
    mIsCycleInProcess(false)
{
    mCurrentCycleClosingState = CycleClosingState::ThreeNodes;

    srand(randomInitializer());
    int timeStarted = (10 * 60) + (rand() % (60 * 60 * 6));
#ifdef TESTS
    timeStarted = kSignalStartTimeSecondsTests;
#endif
    mFiveNodesCycleTimer = make_unique<as::steady_timer>(
                               mIOCtx);
    mFiveNodesCycleTimer->expires_after(
        std::chrono::seconds(
            timeStarted));
    mFiveNodesCycleTimer->async_wait(
        boost::bind(
            &CyclesManager::runSignalFiveNodes,
            this,
            as::placeholders::error));

    timeStarted = (10 * 60) + (rand() % (60 * 60 * 6));
#ifdef TESTS
    timeStarted = kSignalStartTimeSecondsTests;
#endif
    mSixNodesCycleTimer = make_unique<as::steady_timer>(
                              mIOCtx);
    mSixNodesCycleTimer->expires_after(
        std::chrono::seconds(
            timeStarted));
    mSixNodesCycleTimer->async_wait(
        boost::bind(
            &CyclesManager::runSignalSixNodes,
            this,
            as::placeholders::error));

    mUpdatingTimer = make_unique<as::steady_timer>(
                         mIOCtx);
    mUpdatingTimer->expires_after(
        std::chrono::seconds(
            kUpdatingTimerPeriodSeconds));
    mUpdatingTimer->async_wait(
        boost::bind(
            &CyclesManager::updateOfflineNodesAndClosedTLLists,
            this,
            as::placeholders::error));
}

void CyclesManager::addCycle(
    Path::Shared cycle)
{
    if (!mSubsystemsController->isRunCycleClosingTransactions()) {
        debug() << "Adding cycles is forbidden";
        return;
    }
    switch (cycle->length()) {
    case 2:
        debug() << "add three nodes cycle";
        mThreeNodesCycles.push_back(cycle);
        break;
    case 3:
        debug() << "add four nodes cycle";
        mFourNodesCycles.push_back(cycle);
        break;
    case 4:
        debug() << "add five nodes cycle";
        mFiveNodesCycles.push_back(cycle);
        break;
    case 5:
        debug() << "add six nodes cycle";
        mSixNodesCycles.push_back(cycle);
        break;
    default:
        throw ValueError("CyclesManager::addCycle: "
                         "illegal length of cycle");
    }
}

void CyclesManager::closeOneCycle(
    bool nextCycleShouldBeRunned)
{
    if (!mSubsystemsController->isRunCycleClosingTransactions()) {
        debug() << "Closing cycles is forbidden";
        return;
    }
    // nextCycleShouldBeRunned equals true when method closeOneCycle was called
    // by TransactionManager after finishing transaction of closing cycle.
    // When method closeOneCycle was called by transactions of building cycles it equals false
    if (nextCycleShouldBeRunned) {
        mIsCycleInProcess = false;
    }
    clearClosedCycles();
    debug() << "closeOneCycle";
    debug() << "currentCycleClosingState: " << mCurrentCycleClosingState;
    debug() << "3 NC count: " << mThreeNodesCycles.size()
            << " 4 NC count: " << mFourNodesCycles.size()
            << " 5 NC count: " << mFiveNodesCycles.size()
            << " 6 NC count: " << mSixNodesCycles.size();
    if (mIsCycleInProcess) {
        debug() << "Postpone closing this cycle, because another one in process";
        return;
    }
    CycleClosingState currentCycleClosingState = mCurrentCycleClosingState;
    do {
        auto cycles = cyclesVector(
                          mCurrentCycleClosingState);
        if (!cycles->empty()) {
            auto cycle = *cycles->begin();
            cycles->erase(cycles->begin());
            debug() << "closeCycleSignal " << cycle->toString();
            closeCycleSignal(
                mEquivalent,
                cycle);
            mIsCycleInProcess = true;
            return;
        }
        incrementCurrentCycleClosingState();
    } while (currentCycleClosingState != mCurrentCycleClosingState);
    mIsCycleInProcess = false;
}

vector<Path::Shared> *CyclesManager::cyclesVector(
    CycleClosingState currentCycleClosingState)
{
    switch (currentCycleClosingState) {
    case CycleClosingState::ThreeNodes:
        return &mThreeNodesCycles;
    case CycleClosingState::FourNodes:
        return &mFourNodesCycles;
    case CycleClosingState::FiveNodes:
        return &mFiveNodesCycles;
    case CycleClosingState::SixNodes:
        return &mSixNodesCycles;
    default:
        throw ValueError("Unexpected Cycle closing state " + to_string(currentCycleClosingState));
    }
}

void CyclesManager::incrementCurrentCycleClosingState()
{
    switch (mCurrentCycleClosingState) {
    case CycleClosingState::ThreeNodes:
        mCurrentCycleClosingState = CycleClosingState::FourNodes;
        break;
    case CycleClosingState::FourNodes:
        mCurrentCycleClosingState = CycleClosingState::FiveNodes;
        break;
    case CycleClosingState::FiveNodes:
        mCurrentCycleClosingState = CycleClosingState::SixNodes;
        break;
    case CycleClosingState::SixNodes:
        mCurrentCycleClosingState = CycleClosingState::ThreeNodes;
        break;
    }
}

void CyclesManager::runSignalFiveNodes(
    const boost::system::error_code &err)
{
    if (err) {
        warning() << err.message();
    }
    mFiveNodesCycleTimer->cancel();
    auto timeRepeated = kFiveNodesSignalRepeatTimeSeconds;
#ifdef TESTS
    timeRepeated = kSignalRepeatTimeSecondsTests;
#endif
    mFiveNodesCycleTimer->expires_after(
        std::chrono::seconds(
            timeRepeated));
    mFiveNodesCycleTimer->async_wait(
        boost::bind(
            &CyclesManager::runSignalFiveNodes,
            this,
            as::placeholders::error));
    buildFiveNodesCyclesSignal(mEquivalent);
}

void CyclesManager::runSignalSixNodes(
    const boost::system::error_code &err)
{
    if (err) {
        warning() << err.message();
    }
    mSixNodesCycleTimer->cancel();
    auto timeRepeated = kSixNodesSignalRepeatTimeSeconds;
#ifdef TESTS
    timeRepeated = kSignalRepeatTimeSecondsTests;
#endif
    mSixNodesCycleTimer->expires_after(
        std::chrono::seconds(
            timeRepeated));
    mSixNodesCycleTimer->async_wait(
        boost::bind(
            &CyclesManager::runSignalSixNodes,
            this,
            as::placeholders::error));
    buildSixNodesCyclesSignal(mEquivalent);
}

bool CyclesManager::isChallengerTransactionWinReservation(
    BasePaymentTransaction::Shared challengerTransaction,
    BasePaymentTransaction::Shared reservedTransaction)
{
    debug() << "isChallengerTransactionWinReservation challenger: " << challengerTransaction->currentTransactionUUID()
            << " transaction type: " << challengerTransaction->transactionType()
            << " votesCheckingStage: " << challengerTransaction->isVotingStage()
            << " cycle length: " << to_string(challengerTransaction->cycleLength())
            << " coordinator: " << challengerTransaction->coordinatorAddress()->fullAddress();
    debug() << "isChallengerTransactionWinReservation reserved: " << reservedTransaction->currentTransactionUUID()
            << " transaction type: " << reservedTransaction->transactionType()
            << " votesCheckingStage: " << reservedTransaction->isVotingStage()
            << " cycle length: " << to_string(reservedTransaction->cycleLength())
            << " coordinator: " << reservedTransaction->coordinatorAddress()->fullAddress();
    if (reservedTransaction->transactionType() != BaseTransaction::TransactionType::Payments_CycleCloserInitiatorTransaction && reservedTransaction->transactionType() != BaseTransaction::TransactionType::Payments_CycleCloserIntermediateNodeTransaction) {
        debug() << "isChallengerTransactionWinReservation false: reserved is not cycle transaction";
        return false;
    }
    if (reservedTransaction->isVotingStage()) {
        debug() << "isChallengerTransactionWinReservation false: reserved on voting stage";
        return false;
    }
    if (challengerTransaction->cycleLength() != reservedTransaction->cycleLength()) {
        debug() << "isChallengerTransactionWinReservation "
                << (challengerTransaction->cycleLength() > reservedTransaction->cycleLength()) << " on cycles lengths";
        return challengerTransaction->cycleLength() > reservedTransaction->cycleLength();
    }
    debug() << "isChallengerTransactionWinReservation "
            << (challengerTransaction->coordinatorAddress()->fullAddress() > reservedTransaction->coordinatorAddress()->fullAddress())
            << " on coordinatorAddress";
    return challengerTransaction->coordinatorAddress()->fullAddress() > reservedTransaction->coordinatorAddress()->fullAddress();
}

bool CyclesManager::resolveReservationConflict(
    const TransactionUUID &challengerTransactionUUID,
    const TransactionUUID &reservedTransactionUUID)
{
    debug() << "resolveReservationConflict";
    try {
        auto challengerTransaction = static_pointer_cast<BasePaymentTransaction>(
                                         mTransactionScheduler->cycleClosingTransactionByUUID(
                                             challengerTransactionUUID));
        auto reservedTransaction = static_pointer_cast<BasePaymentTransaction>(
                                       mTransactionScheduler->cycleClosingTransactionByUUID(
                                           reservedTransactionUUID));
        debug() << "conflict between  " << challengerTransactionUUID << " and " << reservedTransactionUUID;
        if (isChallengerTransactionWinReservation(
                    challengerTransaction,
                    reservedTransaction)) {
            reservedTransaction->setTransactionState(
                BasePaymentTransaction::Common_RollbackByOtherTransaction);
            mTransactionScheduler->awakeTransaction(
                reservedTransaction);
            return true;
        }
    } catch (NotFoundError &e) {
        warning() << "Can't find transaction " << e.what();
        return false;
    }
    return false;
}

bool CyclesManager::isTransactionStillAlive(
    const TransactionUUID &transactionUUID)
{
    debug() << "isTransactionStillAlive: " << transactionUUID;
    try {
        mTransactionScheduler->cycleClosingTransactionByUUID(
            transactionUUID);
        debug() << "Still alive";
        return true;
    } catch (NotFoundError &e) {
        debug() << "Not alive";
        return false;
    }
}

void CyclesManager::addClosedTrustLine(
    BaseAddress::Shared source,
    BaseAddress::Shared destination)
{
    mClosedTrustLines.insert(
        make_pair(utc_now(),
                  make_pair(
                      source,
                      destination)));
}

void CyclesManager::addOfflineNode(
    BaseAddress::Shared nodeAddress)
{
    mOfflineNodes.insert(
        make_pair(
            utc_now(),
            nodeAddress));
}

void CyclesManager::removeCyclesWithClosedTrustLine(
    BaseAddress::Shared sourceClosed,
    BaseAddress::Shared destinationClosed,
    vector<Path::Shared> &cycles)
{
    auto itCycle = cycles.begin();
    while (itCycle != cycles.end()) {
        if ((*itCycle)->containsTrustLine(
                    sourceClosed,
                    destinationClosed)) {
            cycles.erase(
                itCycle);
        } else {
            itCycle++;
        }
    }
}

void CyclesManager::removeCyclesWithOfflineNode(
    BaseAddress::Shared offlineNode,
    vector<Path::Shared> &cycles)
{
    auto itCycle = cycles.begin();
    while (itCycle != cycles.end()) {
        if ((*itCycle)->positionOfNode(
                    offlineNode) >= 0) {
            cycles.erase(
                itCycle);
        } else {
            itCycle++;
        }
    }
}

void CyclesManager::clearClosedCycles()
{
    debug() << "clearClosedCycles closed trust lines cnt: " << mClosedTrustLines.size();
    if (!mClosedTrustLines.empty()) {
        auto source = mClosedTrustLines.begin()->second.first;
        auto destination = mClosedTrustLines.begin()->second.second;
        mClosedTrustLines.erase(mClosedTrustLines.begin());

        removeCyclesWithClosedTrustLine(
            source,
            destination,
            mThreeNodesCycles);

        removeCyclesWithClosedTrustLine(
            source,
            destination,
            mFourNodesCycles);

        removeCyclesWithClosedTrustLine(
            source,
            destination,
            mFiveNodesCycles);

        removeCyclesWithClosedTrustLine(
            source,
            destination,
            mSixNodesCycles);
    }

    debug() << "clearClosedCycles offline nodes cnt: " << mOfflineNodes.size();
    if (!mOfflineNodes.empty()) {
        auto offlineNode = mOfflineNodes.begin()->second;
        mOfflineNodes.erase(
            mOfflineNodes.begin());

        removeCyclesWithOfflineNode(
            offlineNode,
            mThreeNodesCycles);

        removeCyclesWithOfflineNode(
            offlineNode,
            mFourNodesCycles);

        removeCyclesWithOfflineNode(
            offlineNode,
            mFiveNodesCycles);

        removeCyclesWithOfflineNode(
            offlineNode,
            mSixNodesCycles);
    }
}

void CyclesManager::updateOfflineNodesAndClosedTLLists(
    const boost::system::error_code &err)
{
    if (err) {
        warning() << err.message();
    }
    mUpdatingTimer->cancel();
    mUpdatingTimer->expires_after(
        std::chrono::seconds(
            kUpdatingTimerPeriodSeconds));
    mUpdatingTimer->async_wait(
        boost::bind(
            &CyclesManager::updateOfflineNodesAndClosedTLLists,
            this,
            as::placeholders::error));

    // delete legacy offline nodes
    for (auto timeAndNode : mOfflineNodes) {
        if (utc_now() - timeAndNode.first > kOfflineNodesAndClosedTLLiveDuration()) {
            mOfflineNodes.erase(timeAndNode.first);
        } else {
            break;
        }
    }

    // delete legacy closed TL
    for (auto timeAndTrustLine : mClosedTrustLines) {
        if (utc_now() - timeAndTrustLine.first > kOfflineNodesAndClosedTLLiveDuration()) {
            mClosedTrustLines.erase(timeAndTrustLine.first);
        } else {
            break;
        }
    }
}

uint32_t CyclesManager::randomInitializer() const
{
    NodeUUID randomInitializator;
    uint32_t result = 0;
    for (int i = 0; i < NodeUUID::kBytesSize; i++) {
        result = result << 2;
        byte_t tmp = (byte_t)(randomInitializator.data[i] * mEquivalent);
        result |= (tmp & 0x3);
    }
    return result;
}

LoggerStream CyclesManager::info() const
{
    return mLog.info(logHeader());
}

LoggerStream CyclesManager::debug() const
{
    return mLog.debug(logHeader());
}

LoggerStream CyclesManager::warning() const
{
    return mLog.warning(logHeader());
}

const string CyclesManager::logHeader() const
{
    stringstream s;
    s << "CyclesManager: " << mEquivalent << " ";
    return s.str();
}
