#ifndef VTCPD_RUNTIMEERROR_H
#define VTCPD_RUNTIMEERROR_H

#include "Exception.h"


class RuntimeError:
    public Exception
{

public:
    using Exception::Exception;
};

#endif //VTCPD_RUNTIMEERROR_H
