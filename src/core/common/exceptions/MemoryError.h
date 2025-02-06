#ifndef VTCPD_MEMORYERROR_H
#define VTCPD_MEMORYERROR_H

#include "Exception.h"


class MemoryError: public Exception
{
    using Exception::Exception;
};

#endif //VTCPD_MEMORYERROR_H
