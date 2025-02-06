#ifndef VTCPD_SETCHANNELCONTRACTORADDRESSESCOMMAND_H
#define VTCPD_SETCHANNELCONTRACTORADDRESSESCOMMAND_H

#include "../BaseUserCommand.h"

class SetChannelContractorAddressesCommand : public BaseUserCommand
{

public:
    typedef shared_ptr<SetChannelContractorAddressesCommand> Shared;

public:
    SetChannelContractorAddressesCommand(
        const CommandUUID &commandUUID,
        const string &commandBuffer);

    static const string &identifier();

    vector<BaseAddress::Shared> contractorAddresses() const;

    const ContractorID contractorChannelID() const;

private:
    vector<BaseAddress::Shared> mContractorAddresses;
    ContractorID mContractorChannelID;
};


#endif //VTCPD_SETCHANNELCONTRACTORADDRESSESCOMMAND_H
