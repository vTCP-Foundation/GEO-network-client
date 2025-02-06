#ifndef VTCPD_CHECKTRUSTLINETRANSACTION_H
#define VTCPD_CHECKTRUSTLINETRANSACTION_H

#include "../base/BaseTransaction.h"
#include "../../../trust_lines/manager/TrustLinesManager.h"

class CheckTrustLineTransaction : public BaseTransaction
{

public:
    typedef shared_ptr<CheckTrustLineTransaction> Shared;

public:
    CheckTrustLineTransaction(
        const SerializedEquivalent equivalent,
        ContractorID contractorID,
        bool isActionInitiator,
        TrustLinesManager *manager,
        Logger &logger);

    TransactionResult::SharedConst run() override;

protected:
    const string logHeader() const override;

private:
    ContractorID mContractorID;
    bool mIsActionInitiator;
    TrustLinesManager *mTrustLinesManager;
};


#endif //VTCPD_CHECKTRUSTLINETRANSACTION_H
