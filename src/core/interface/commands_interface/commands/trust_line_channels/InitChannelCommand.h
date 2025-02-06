#ifndef VTCPD_INITCHANNELCOMMAND_H
#define VTCPD_INITCHANNELCOMMAND_H

#include "../BaseUserCommand.h"

class InitChannelCommand : public BaseUserCommand
{

public:
    typedef shared_ptr<InitChannelCommand> Shared;

public:
    InitChannelCommand(
        const CommandUUID &commandUUID,
        const string &commandBuffer);

    static const string &identifier();

    vector<BaseAddress::Shared> contractorAddresses() const;

    const string &cryptoKey() const;

    const ContractorID contractorChannelID() const;

    CommandResult::SharedConst responseOk(
        string &channelInfo) const;

private:
    vector<BaseAddress::Shared> mContractorAddresses;
    string mCryptoKey;
    ContractorID mContractorChannelID;
};


#endif //VTCPD_INITCHANNELCOMMAND_H
