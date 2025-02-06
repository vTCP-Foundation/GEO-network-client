#ifndef VTCPD_FIRSTLEVELCONTRACTORSBALANCESCOMMAND_H
#define VTCPD_FIRSTLEVELCONTRACTORSBALANCESCOMMAND_H

#include "../BaseUserCommand.h"

class GetTrustLinesCommand : public BaseUserCommand
{

public:
    typedef shared_ptr<GetTrustLinesCommand> Shared;

public:
    GetTrustLinesCommand(
        const CommandUUID &uuid,
        const string &commandBuffer);

    static const string &identifier();

    const size_t from() const;

    const size_t count() const;

    const SerializedEquivalent equivalent() const;

    CommandResult::SharedConst resultOk(
        string &neighbors) const;

private:
    SerializedEquivalent mEquivalent;
    size_t mFrom;
    size_t mCount;
};

#endif //VTCPD_FIRSTLEVELCONTRACTORSBALANCESCOMMAND_H
