#include "MaxFlowCalculationTargetFstLevelTransaction.h"

MaxFlowCalculationTargetFstLevelTransaction::MaxFlowCalculationTargetFstLevelTransaction(
    MaxFlowCalculationTargetFstLevelMessage::Shared message,
    ContractorsManager *contractorsManager,
    TrustLinesManager *manager,
    TopologyCacheManager *topologyCacheManager,
    Logger &logger,
    bool iAmGateway) :

    BaseTransaction(
        BaseTransaction::MaxFlowCalculationTargetFstLevelTransactionType,
        message->equivalent(),
        logger),
    mMessage(message),
    mContractorsManager (contractorsManager),
    mTrustLinesManager(manager),
    mTopologyCacheManager(topologyCacheManager),
    mIAmGateway(iAmGateway)
{}

TransactionResult::SharedConst MaxFlowCalculationTargetFstLevelTransaction::run()
{
#ifdef DEBUG_LOG_MAX_FLOW_CALCULATION
    info() << "run\t" << "sender: " << mMessage->idOnReceiverSide;
    info() << "run\t" << "target: " << mMessage->targetAddresses().at(0)->fullAddress();
    info() << "run\t" << "i am is gateway: " << mIAmGateway;
    info() << "run\t" << "OutgoingFlows: " << mTrustLinesManager->outgoingFlows().size();
    info() << "run\t" << "IncomingFlows: " << mTrustLinesManager->incomingFlows().size();
#endif
    pair<vector<ContractorID>, vector<ContractorID>> incomingFlowIDs;
    if (mIAmGateway) {
        vector<pair<BaseAddress::Shared, ConstSharedTrustLineAmount>> outgoingFlows;
        vector<pair<BaseAddress::Shared, ConstSharedTrustLineAmount>> incomingFlows;
        // inform that I am is gateway
        sendMessage<ResultMaxFlowCalculationGatewayMessage>(
            mMessage->targetAddresses().at(0),
            mEquivalent,
            mContractorsManager->ownAddresses(),
            outgoingFlows,
            incomingFlows);
        if (mMessage->isTargetGateway()) {
            incomingFlowIDs = mTrustLinesManager->firstLevelGatewayNeighborsWithIncomingFlow();
        } else {
            incomingFlowIDs = mTrustLinesManager->firstLevelNeighborsWithIncomingFlow();
        }
    } else {
        incomingFlowIDs = mTrustLinesManager->firstLevelNonGatewayNeighborsWithIncomingFlow();
    }
    auto targetContractorID = mContractorsManager->contractorIDByAddress(
                                  mMessage->targetAddresses().at(0));
    for (auto const &nodeIDWithIncomingFlow : incomingFlowIDs.first) {
        if (nodeIDWithIncomingFlow == mMessage->idOnReceiverSide or
                nodeIDWithIncomingFlow == targetContractorID) {
            continue;
        }
#ifdef DEBUG_LOG_MAX_FLOW_CALCULATION
        info() << "sendFirst: " << nodeIDWithIncomingFlow;
#endif
        sendMessage<MaxFlowCalculationTargetSndLevelMessage>(
            nodeIDWithIncomingFlow,
            mEquivalent,
            mContractorsManager->idOnContractorSide(nodeIDWithIncomingFlow),
            mMessage->targetAddresses(),
            mMessage->isTargetGateway());
        mTopologyCacheManager->addIntoFirstLevelCache(
            nodeIDWithIncomingFlow);
    }
    for (auto const &nodeIDWithIncomingFlow : incomingFlowIDs.second) {
        if (nodeIDWithIncomingFlow == mMessage->idOnReceiverSide or
                nodeIDWithIncomingFlow == targetContractorID) {
            continue;
        }
        if (!mTopologyCacheManager->isInFirstLevelCache(nodeIDWithIncomingFlow)) {
            continue;
        }
#ifdef DEBUG_LOG_MAX_FLOW_CALCULATION
        info() << "sendFirst zero: " << nodeIDWithIncomingFlow;
#endif
        sendMessage<MaxFlowCalculationTargetSndLevelMessage>(
            nodeIDWithIncomingFlow,
            mEquivalent,
            mContractorsManager->idOnContractorSide(nodeIDWithIncomingFlow),
            mMessage->targetAddresses(),
            mMessage->isTargetGateway());
    }
    return resultDone();
}

const string MaxFlowCalculationTargetFstLevelTransaction::logHeader() const
{
    stringstream s;
    s << "[MaxFlowCalculationTargetFstLevelTA: " << currentTransactionUUID() << " " << mEquivalent << "]";
    return s.str();
}
