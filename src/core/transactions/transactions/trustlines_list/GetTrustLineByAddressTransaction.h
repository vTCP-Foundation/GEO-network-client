#ifndef VTCPD_GETTRUSTLINEBYADDRESSTRANSACTION_H_H
#define VTCPD_GETTRUSTLINEBYADDRESSTRANSACTION_H_H

#include "../base/BaseTransaction.h"
#include "../../../interface/commands_interface/commands/trust_lines_list/GetTrustLineByAddressCommand.h"
#include "../../../contractors/ContractorsManager.h"
#include "../../../trust_lines/manager/TrustLinesManager.h"

class GetTrustLineByAddressTransaction :
    public BaseTransaction
{

public:
    typedef shared_ptr<GetTrustLineByAddressTransaction> Shared;

public:
    GetTrustLineByAddressTransaction(
        GetTrustLineByAddressCommand::Shared command,
        ContractorsManager *contractorsManager,
        TrustLinesManager *trustLinesManager,
        Logger &logger)
    noexcept;

    TransactionResult::SharedConst run() override;

protected:
    TransactionResult::SharedConst resultTrustLineIsAbsent();

    const string logHeader() const override;

private:
    GetTrustLineByAddressCommand::Shared mCommand;
    ContractorsManager *mContractorsManager;
    TrustLinesManager *mTrustLinesManager;
};

#endif //VTCPD_GETTRUSTLINEBYADDRESSTRANSACTION_H_H
