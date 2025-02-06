#ifndef VTCPD_CONTRACTORLISTCOMMAND_H
#define VTCPD_CONTRACTORLISTCOMMAND_H

#include "../BaseUserCommand.h"

class ContractorListCommand : public BaseUserCommand
{

public:
    typedef shared_ptr<ContractorListCommand> Shared;

public:
    ContractorListCommand(
        const CommandUUID &uuid,
        const string &commandBuffer);

    static const string &identifier();

    CommandResult::SharedConst resultOk(
        string &contractors) const;
};

#endif //VTCPD_CONTRACTORLISTCOMMAND_H
