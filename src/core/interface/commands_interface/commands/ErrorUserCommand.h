#ifndef VTCPD_COMMAND_ERROR_H
#define VTCPD_COMMAND_ERROR_H

#include "BaseUserCommand.h"

class ErrorUserCommand : public BaseUserCommand
{

public:
    ErrorUserCommand(
        const CommandUUID &commandUUID,
        const string& identifier,
        const string& error) :
        BaseUserCommand(
            commandUUID,
            identifier),
        mError(error)
    {}

    inline const string &error() const
    {
        return mError;
    }

    CommandResult::SharedConst responseError() const
    {
        return CommandResult::SharedConst(
                   new CommandResult(
                       identifier(),
                       UUID(),
                       606,
                       mError));
    }

protected:
    std::string mError;
};

#endif //VTCPD_COMMAND_ERROR_H
