#ifndef VTCPD_GETEQUIVALENTLISTTRANSACTION_H
#define VTCPD_GETEQUIVALENTLISTTRANSACTION_H

#include "../base/BaseTransaction.h"
#include "../../../interface/commands_interface/commands/trust_lines_list/EquivalentListCommand.h"
#include "../../../equivalents/EquivalentsSubsystemsRouter.h"

class GetEquivalentListTransaction : public BaseTransaction
{

public:
    typedef shared_ptr<GetEquivalentListTransaction> Shared;

public:
    GetEquivalentListTransaction(
        EquivalentListCommand::Shared command,
        EquivalentsSubsystemsRouter *equivalentsSubsystemsRouter,
        Logger &logger)
    noexcept;

    TransactionResult::SharedConst run() override;

protected:
    const string logHeader() const override;

private:
    EquivalentListCommand::Shared mCommand;
    EquivalentsSubsystemsRouter *mEquivalentsSubsystemsRouter;
};


#endif //VTCPD_GETEQUIVALENTLISTTRANSACTION_H
