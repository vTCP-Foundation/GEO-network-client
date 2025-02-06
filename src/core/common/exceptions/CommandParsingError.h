#ifndef VTCPD_COMMANDPARSINGERROR_H
#define VTCPD_COMMANDPARSINGERROR_H

#include "Exception.h"

// todo: replace with ValueError
class CommandParsingError : public Exception
{
    using Exception::Exception;
};

#endif //VTCPD_COMMANDPARSINGERROR_H
