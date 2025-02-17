#ifndef VTCPD_GETTRUSTLINESLISTTRANSACTION_H
#define VTCPD_GETTRUSTLINESLISTTRANSACTION_H

#include "../base/BaseTransaction.h"
#include "../../../interface/commands_interface/commands/trust_lines_list/GetTrustLinesCommand.h"
#include "../../../trust_lines/manager/TrustLinesManager.h"
#include "../../../contractors/ContractorsManager.h"

class GetTrustLinesListTransaction :
    public BaseTransaction
{

public:
    typedef shared_ptr<GetTrustLinesListTransaction> Shared;

public:
    GetTrustLinesListTransaction(
        GetTrustLinesCommand::Shared command,
        TrustLinesManager *trustLinesManager,
        ContractorsManager* contractorsManager,
        Logger &logger)
    noexcept;

    TransactionResult::SharedConst run() override;

protected:
    const string logHeader() const override;

private:
    GetTrustLinesCommand::Shared mCommand;
    TrustLinesManager *mTrustLinesManager;
    ContractorsManager *mContractorsManager;
};

#endif //VTCPD_GETTRUSTLINESLISTTRANSACTION_H
