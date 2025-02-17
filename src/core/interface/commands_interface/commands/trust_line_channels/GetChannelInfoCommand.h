#ifndef VTCPD_GETCHANNELINFOCOMMAND_H
#define VTCPD_GETCHANNELINFOCOMMAND_H

#include "../BaseUserCommand.h"

class GetChannelInfoCommand : public BaseUserCommand
{

public:
    typedef shared_ptr<GetChannelInfoCommand> Shared;

public:
    GetChannelInfoCommand(
        const CommandUUID &commandUUID,
        const string &commandBuffer);

    static const string &identifier();

    const ContractorID contractorID() const;

    CommandResult::SharedConst resultOk(
        string &neighbor) const;

private:
    ContractorID mContractorID;
};


#endif //VTCPD_GETCHANNELINFOCOMMAND_H
