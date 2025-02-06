#ifndef VTCPD_INITTRUSTLINECOMMAND_H
#define VTCPD_INITTRUSTLINECOMMAND_H

#include "../BaseUserCommand.h"

class InitTrustLineCommand : public BaseUserCommand
{

public:
    typedef shared_ptr<InitTrustLineCommand> Shared;

public:
    InitTrustLineCommand(
        const CommandUUID &commandUUID,
        const string &commandBuffer);

    static const string &identifier();

    const ContractorID contractorID() const;

    const SerializedEquivalent equivalent() const;

private:
    SerializedEquivalent mEquivalent;
    ContractorID mContractorID;
};

#endif //VTCPD_INITTRUSTLINECOMMAND_H
