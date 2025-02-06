#ifndef VTCPD_PRECONDITIONFAULTERROR_H
#define VTCPD_PRECONDITIONFAULTERROR_H

#include "Exception.h"


class PreconditionFailedError: public Exception
{
    using Exception::Exception;
};

#endif //VTCPD_PRECONDITIONFAULTERROR_H
