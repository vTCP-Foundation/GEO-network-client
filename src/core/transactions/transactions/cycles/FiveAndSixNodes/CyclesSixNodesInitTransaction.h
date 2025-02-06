#ifndef VTCPD_CYCLESSIXNODESINITTRANSACTION_H
#define VTCPD_CYCLESSIXNODESINITTRANSACTION_H

#include "base/CyclesBaseFiveSixNodesInitTransaction.h"
#include "../../../../network/messages/cycles/SixAndFiveNodes/CyclesSixNodesInBetweenMessage.hpp"
#include "../../../../network/messages/cycles/SixAndFiveNodes/CyclesSixNodesBoundaryMessage.hpp"

class CyclesSixNodesInitTransaction :
    public CyclesBaseFiveSixNodesInitTransaction
{

public:
    CyclesSixNodesInitTransaction(
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
#endif //VTCPD_CYCLESSIXNODESINITTRANSACTION_H
