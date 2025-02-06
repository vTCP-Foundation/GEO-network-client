#include "TrustLinesInfluenceCommand.h"

TrustLinesInfluenceCommand::TrustLinesInfluenceCommand(
    const CommandUUID &uuid,
    const string &commandBuffer) :

    BaseUserCommand(
        uuid,
        identifier())
{
    auto check = [&](auto &ctx) {
        if(_attr(ctx) == kCommandsSeparator || _attr(ctx) == kTokensSeparator) {
            throw ValueError("TrustLinesInfluenceCommand: input is empty.");
        }
    };
    auto flagsParse = [&](auto &ctx) {
        mFlags = _attr(ctx);
        mFirstParameter = 0;
        mSecondParameter = 0;
        mThirdParameter = 0;
    };
    auto firstParamParse = [&](auto &ctx) {
        mFirstParameter = _attr(ctx);
    };
    auto secondParamParse = [&](auto &ctx) {
        mSecondParameter = _attr(ctx);
    };
    auto thirdParamParse = [&](auto &ctx) {
        mThirdParameter = _attr(ctx);
    };

    try {
        parse(
            commandBuffer.begin(),
            commandBuffer.end(),
            char_[check]);
        parse(
            commandBuffer.begin(),
            commandBuffer.end(), (
                *(int_[flagsParse])
                >-(char_(kTokensSeparator)
                   >*(int_[firstParamParse])
                   >char_(kTokensSeparator)
                   >*(int_[secondParamParse])
                   >char_(kTokensSeparator)
                   >*(int_[thirdParamParse]))
                >eol > eoi));
    } catch(...) {
        throw ValueError("TrustLinesInfluenceCommand: cannot parse command.");
    }
}

const string& TrustLinesInfluenceCommand::identifier()
{
    static const string identifier = "SET:subsystems_controller/trust_lines_influence/flags";
    return identifier;
}

size_t TrustLinesInfluenceCommand::flags() const
{
    return mFlags;
}

const uint32_t TrustLinesInfluenceCommand::firstParameter() const
{
    return mFirstParameter;
}

const uint32_t TrustLinesInfluenceCommand::secondParameter() const
{
    return mSecondParameter;
}

const uint32_t TrustLinesInfluenceCommand::thirdParameter() const
{
    return mThirdParameter;
}