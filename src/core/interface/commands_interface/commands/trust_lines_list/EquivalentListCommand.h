#ifndef VTCPD_EQUIVALENTLISTCOMMAND_H
#define VTCPD_EQUIVALENTLISTCOMMAND_H

#include "../BaseUserCommand.h"

class EquivalentListCommand : public BaseUserCommand
{

public:
    typedef shared_ptr<EquivalentListCommand> Shared;

public:
    EquivalentListCommand(
        const CommandUUID &uuid,
        const string &commandBuffer);

    static const string &identifier();

    CommandResult::SharedConst resultOk(
        string &equivalents) const;
};

#endif //VTCPD_EQUIVALENTLISTCOMMAND_H
