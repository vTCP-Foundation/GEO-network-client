#include "TotalBalancesCommand.h"

TotalBalancesCommand::TotalBalancesCommand(
    const CommandUUID &uuid,
    const string &commandBuffer):

    BaseUserCommand(
        uuid,
        identifier())
{
    auto check = [&](auto &ctx) {
        if(_attr(ctx) == kCommandsSeparator || _attr(ctx) == kTokensSeparator) {
            throw ValueError("TotalBalancesCommand: input is empty.");
        }
    };
    auto equivalentParse = [&](auto &ctx) {
        mEquivalent = _attr(ctx);
    };

    try {
        parse(
            commandBuffer.begin(),
            commandBuffer.end(),
            char_[check]);
        parse(
            commandBuffer.begin(),
            commandBuffer.end(),
            *(int_[equivalentParse]) > eol > eoi);
    } catch (...) {
        throw ValueError("TotalBalancesCommand: cannot parse command.");
    }
}

const string &TotalBalancesCommand::identifier()
{
    static const string identifier = "GET:stats/balance/total";
    return identifier;
}

const SerializedEquivalent TotalBalancesCommand::equivalent() const
{
    return mEquivalent;
}

CommandResult::SharedConst TotalBalancesCommand::resultOk(
    string &totalBalancesStr) const
{
    return CommandResult::SharedConst(
               new CommandResult(
                   identifier(),
                   UUID(),
                   200,
                   totalBalancesStr));
}
