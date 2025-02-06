#include "InitiateMaxFlowCalculationCommand.h"

InitiateMaxFlowCalculationCommand::InitiateMaxFlowCalculationCommand(
    const CommandUUID &uuid,
    const string &command):

    BaseUserCommand(
        uuid,
        identifier())
{
    std::string address, addressType;
    auto check = [&](auto &ctx) {
        if(_attr(ctx) == kCommandsSeparator || _attr(ctx) == kTokensSeparator) {
            throw ValueError("InitiateMaxFlowCalculationCommand: input is empty.");
        }
    };
    auto addressTypeParse = [&](auto &ctx) {
        addressType += _attr(ctx);
    };
    auto addressAddChar = [&](auto &ctx) {
        address += _attr(ctx);
    };
    auto addressAddNumber = [&](auto &ctx) {
        address += std::to_string(_attr(ctx));
    };
    auto addressesCountParse = [&](auto &ctx) {
        mContractorsCount = _attr(ctx);
    };
    auto addressAddToVector = [&](auto &ctx) {
        switch (std::atoi(addressType.c_str())) {
        case BaseAddress::IPv4_IncludingPort: {
            mContractorAddresses.push_back(
                make_shared<IPv4WithPortAddress>(
                    address));
            addressType.erase();
            break;
        }
        case BaseAddress::GNS: {
            mContractorAddresses.push_back(
                make_shared<GNSAddress>(
                    address));
            addressType.erase();
            break;
        }
        default:
            throw ValueError("InitiateMaxFlowCalculationCommand: cannot parse command. "
                             "Error occurred while parsing 'Contractor Address' token.");
        }
        address.erase();
    };
    auto equivalentParse = [&](auto &ctx) {
        mEquivalent = _attr(ctx);
    };

    try {
        parse(
            command.begin(),
            command.end(),
            char_[check]);
        parse(
            command.begin(),
            command.end(),
            *(int_[addressesCountParse]-char_(kTokensSeparator)) > char_(kTokensSeparator));
        mContractorAddresses.reserve(mContractorsCount);
        parse(
            command.begin(),
            command.end(),
            *(int_)
            > char_(kTokensSeparator)
            > addressLexeme<
            decltype(addressAddChar),
            decltype(addressAddNumber),
            decltype(addressTypeParse),
            decltype(addressAddToVector)>(
                mContractorsCount,
                addressAddChar,
                addressAddNumber,
                addressTypeParse,
                addressAddToVector)
            > +(int_[equivalentParse]) > eol > eoi);
    } catch(...) {
        throw ValueError("InitTrustLineCommand: cannot parse command.");
    }
}

const string &InitiateMaxFlowCalculationCommand::identifier()
{
    static const string identifier = "GET:contractors/transactions/max";
    return identifier;
}

const vector<BaseAddress::Shared>& InitiateMaxFlowCalculationCommand::contractorAddresses() const
{
    return mContractorAddresses;
}

const SerializedEquivalent InitiateMaxFlowCalculationCommand::equivalent() const
{
    return mEquivalent;
}

CommandResult::SharedConst InitiateMaxFlowCalculationCommand::responseOk(
    string &maxFlowAmount) const
{
    return CommandResult::SharedConst(
               new CommandResult(
                   identifier(),
                   UUID(),
                   200,
                   maxFlowAmount));
}