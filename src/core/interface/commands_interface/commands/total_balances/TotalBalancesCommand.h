#ifndef VTCPD_TOTALBALANCESCOMMAND_H
#define VTCPD_TOTALBALANCESCOMMAND_H

#include "../BaseUserCommand.h"

class TotalBalancesCommand : public BaseUserCommand
{

public:
    typedef shared_ptr<TotalBalancesCommand> Shared;

public:
    TotalBalancesCommand(
        const CommandUUID &uuid,
        const string &commandBuffer);

    static const string &identifier();

    const SerializedEquivalent equivalent() const;

    CommandResult::SharedConst resultOk(
        string &totalBalancesStr) const;

private:
    SerializedEquivalent mEquivalent;
};

#endif //VTCPD_TOTALBALANCESCOMMAND_H
