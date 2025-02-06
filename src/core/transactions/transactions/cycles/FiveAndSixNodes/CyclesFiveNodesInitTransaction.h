#ifndef GEO_NETWORK_CLIENT_CYCLESFIVENODESINITTRANSACTION_H
#define GEO_NETWORK_CLIENT_CYCLESFIVENODESINITTRANSACTION_H

#include "base/CyclesBaseFiveSixNodesInitTransaction.h"
#include "../../../../network/messages/cycles/SixAndFiveNodes/CyclesFiveNodesInBetweenMessage.hpp"
#include "../../../../network/messages/cycles/SixAndFiveNodes/CyclesFiveNodesBoundaryMessage.hpp"
#include "../../../../common/multiprecision/MultiprecisionUtils.h"

class CyclesFiveNodesInitTransaction :
    public CyclesBaseFiveSixNodesInitTransaction
{

public:
    CyclesFiveNodesInitTransaction(
        const SerializedEquivalent equivalent,
        ContractorsManager *contractorsManager,
        TrustLinesManager *trustLinesManager,
        CyclesManager *cyclesManager,
        TailManager *tailManager,
        Logger &logger);

protected:
    const string logHeader() const override;

    TransactionResult::SharedConst runCollectDataAndSendMessagesStage() override;
    TransactionResult::SharedConst runParseMessageAndCreateCyclesStage() override;
};
#endif //GEO_NETWORK_CLIENT_CYCLESFIVENODESINITTRANSACTION_H
