#ifndef VTCPD_INITIATEMAXFLOWCALCULATIONCOMMAND_H
#define VTCPD_INITIATEMAXFLOWCALCULATIONCOMMAND_H

#include "../BaseUserCommand.h"

class InitiateMaxFlowCalculationCommand : public BaseUserCommand
{

public:
    typedef shared_ptr<InitiateMaxFlowCalculationCommand> Shared;

public:
    InitiateMaxFlowCalculationCommand(
        const CommandUUID &uuid,
        const string &command);

    static const string &identifier();

    const vector<BaseAddress::Shared> &contractorAddresses() const;

    const SerializedEquivalent equivalent() const;

    CommandResult::SharedConst responseOk(
        string &maxFlowAmount) const;

private:
    size_t mContractorsCount;
    vector<BaseAddress::Shared> mContractorAddresses;
    SerializedEquivalent mEquivalent;
};

#endif //VTCPD_INITIATEMAXFLOWCALCULATIONCOMMAND_H
