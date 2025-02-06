#ifndef GEO_NETWORK_CLIENT_CYCLECLOSERINITIATORTRANSACTION_H
#define GEO_NETWORK_CLIENT_CYCLECLOSERINITIATORTRANSACTION_H

#include "base/BasePaymentTransaction.h"
#include "../../../../cycles/CyclesManager.h"

class CycleCloserInitiatorTransaction : public BasePaymentTransaction
{

public:
    typedef shared_ptr<CycleCloserInitiatorTransaction> Shared;
    typedef shared_ptr<const CycleCloserInitiatorTransaction> ConstShared;

public:
    CycleCloserInitiatorTransaction(
        const Path::Shared path,
        const SerializedEquivalent equivalent,
        ContractorsManager *contractorsManager,
        TrustLinesManager *trustLines,
        CyclesManager *cyclesManager,
        StorageHandler *storageHandler,
        TopologyCacheManager *topologyCacheManager,
        MaxFlowCacheManager *maxFlowCacheManager,
        ResourcesManager *resourcesManager,
        Keystore *keystore,
        Logger &log,
        SubsystemsController *subsystemsController);

    TransactionResult::SharedConst run() override;

    /**
     * @return length of cycle which is closing by current transaction
     * used in CyclesManager for resolving cycle closing conflicts
     */
    const SerializedPathLengthSize cycleLength() const override;

protected:
    // Stages handlers
    // TODO: Add throws specifications
    /**
     * check if cycle is valid and current node has enough amount to close it
     */
    TransactionResult::SharedConst runInitializationStage();

    /**
     * process the reservation of transaction amount on cycle path
     */
    TransactionResult::SharedConst runAmountReservationStage();

    /**
     * reaction on reservation request message from last intermediate node in path
     */
    TransactionResult::SharedConst runPreviousNeighborRequestProcessingStage();

    /**
     * continue process the reservation of transaction amount on cycle path
     * after waiting for closing conflicted transaction and releasing it amount
     */
    TransactionResult::SharedConst runAmountReservationStageAgain();

    /**
     * continue reaction on reservation request message from last intermediate node in path
     * after waiting for closing conflicted transaction and releasing it amount
     */
    TransactionResult::SharedConst runPreviousNeighborRequestProcessingStageAgain();

    /**
     * reaction on messages with approving or not of final amounts configuration from all participants
     */
    TransactionResult::SharedConst runFinalAmountsConfigurationConfirmationProcessingStage();

    TransactionResult::SharedConst runFinalAmountsParticipantConfirmation();

    TransactionResult::SharedConst runFinalReservationsNeighborConfirmation();

    /**
     * reaction on receiving participants votes message with result of voting,
     * on this stage node can commit transaction or reject it
     * and send result to all participants
     */
    TransactionResult::SharedConst runVotesConsistencyCheckingStage() override;

    /**
     * rollback current transaction because of cycle closing conflict
     */
    TransactionResult::SharedConst runRollbackByOtherTransactionStage();

protected:
    /**
     * try reserve available amount to next node on closing cycle
     */
    TransactionResult::SharedConst tryReserveNextIntermediateNodeAmount();

    /**
     * send reservation request to neighbor on closing cycle
     */
    TransactionResult::SharedConst askNeighborToReserveAmount();

    /**
     * reaction on reservation response from neighbor
     */
    TransactionResult::SharedConst processNeighborAmountReservationResponse();

    /**
     * send further reservation request to neighbor on closing cycle
     * (neighbor should reserve amount to his neighbor)
     */
    TransactionResult::SharedConst askNeighborToApproveFurtherNodeReservation();

    /**
     * reaction on further reservation response from neighbor
     */
    TransactionResult::SharedConst processNeighborFurtherReservationResponse();

    /**
     * send further reservation request to remote intermediate node (node should reserve amount to his neighbor)
     * @param remoteNode node to which current node send request
     * @param remoteNodePosition position of remote node in pathStats
     * @param nextNodeAfterRemote neighbor of remote node to which it should reserve available amount
     */
    TransactionResult::SharedConst askRemoteNodeToApproveReservation(
        BaseAddress::Shared remoteNode,
        const byte_t remoteNodePosition,
        BaseAddress::Shared nextNodeAfterRemote);

    /**
     * reaction on further reservation response from remote node
     */
    TransactionResult::SharedConst processRemoteNodeResponse();

    /**
     * send messages to all transaction participants with final amount which should be committed
     */
    TransactionResult::SharedConst sendFinalPathConfigurationToAllParticipants();

    TransactionResult::SharedConst propagateVotesListAndWaitForVotingResult();

protected:
    TransactionResult::SharedConst approve() override;

protected:
    const string logHeader() const override;

    /**
     * check if path is valid cycle
     * @param path closing cycle
     */
    void checkPath(
        const Path::ConstShared path);

    /**
     * send messages to intermediate nodes with instruction "finish transaction"
     * @param lastInformedNodePosition position of last intermediate node in closing cycle
     * which will be informed about transaction finishing
     */
    void informIntermediateNodesAboutTransactionFinish(
        const SerializedPositionInPath lastInformedNodePosition);

    /**
     * save result of payment transaction on database
     * @param ioTransaction pointer on database transaction
     */
    void savePaymentOperationIntoHistory(
        IOTransaction::Shared ioTransaction) override;

    /**
     * check if reservations on current node are valid before committing
     * (there are should be one incoming and one outgoing reservations with the same amount)
     * @return true if reservations are valid
     */
    bool checkReservationsDirections() const override;

protected:
    // Contains special stats data, such as current max flow,
    // for closing cycle involved into the transaction.
    unique_ptr<PathStats> mPathStats;

    // Contains all addresses of participants of current path.
    // Only main addresses are used for building paths.
    // During reservations all participants inform coordinator about theirs all addresses.
    vector<Contractor::Shared> mCurrentPathParticipants;

    // fields, for continue process coordinator request after releasing conflicted reservation
    // transaction on which reservation we pretend
    TransactionUUID mConflictedTransaction;
    BaseAddress::Shared mNextNode;
    BaseAddress::Shared mPreviousNode;
    ContractorID mNextNodeID;
    ContractorID mPreviousNodeID;
    TrustLineAmount mOutgoingAmount;
    TrustLineAmount mIncomingAmount;

    // for resolving reservation conflicts
    CyclesManager *mCyclesManager;

private:
    // time waiting (in milliseconds) for closing of conflicted transaction
    const uint16_t kWaitingForReleasingAmountMSec = 50;
};

#endif // GEO_NETWORK_CLIENT_CYCLECLOSERINITIATORTRANSACTION_H
