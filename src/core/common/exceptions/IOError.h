#ifndef VTCPD_IOERROR_H
#define VTCPD_IOERROR_H

#include "Exception.h"


class IOError: public Exception
{
    using Exception::Exception;
};


#endif //VTCPD_IOERROR_H
