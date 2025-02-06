#ifndef VTCPD_GETCHANNELINFOBYADDRESSESCOMMAND_H
#define VTCPD_GETCHANNELINFOBYADDRESSESCOMMAND_H

#include "../BaseUserCommand.h"

class GetChannelInfoByAddressesCommand : public BaseUserCommand
{

public:
    typedef shared_ptr<GetChannelInfoByAddressesCommand> Shared;

public:
    GetChannelInfoByAddressesCommand(
        const CommandUUID &uuid,
        const string &commandBuffer);

    static const string &identifier();

    vector<BaseAddress::Shared> contractorAddresses() const;

    CommandResult::SharedConst resultOk(
        string &neighbor) const;

private:
    vector<BaseAddress::Shared> mContractorAddresses;
};

#endif //VTCPD_GETCHANNELINFOBYADDRESSESCOMMAND_H
