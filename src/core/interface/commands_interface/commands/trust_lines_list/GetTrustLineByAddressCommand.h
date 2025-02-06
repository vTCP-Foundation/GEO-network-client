#ifndef VTCPD_GETTRUSTLINEBYADDRESSCOMMAND_H
#define VTCPD_GETTRUSTLINEBYADDRESSCOMMAND_H

#include "../BaseUserCommand.h"

class GetTrustLineByAddressCommand : public BaseUserCommand
{

public:
    typedef shared_ptr<GetTrustLineByAddressCommand> Shared;

public:
    GetTrustLineByAddressCommand(
        const CommandUUID &uuid,
        const string &commandBuffer);

    static const string &identifier();

    vector<BaseAddress::Shared> contractorAddresses() const;

    const SerializedEquivalent equivalent() const;

    CommandResult::SharedConst resultOk(
        string &neighbor) const;

protected:
    vector<BaseAddress::Shared> mContractorAddresses;
    SerializedEquivalent mEquivalent;
};

#endif //VTCPD_GETTRUSTLINEBYADDRESSCOMMAND_H
