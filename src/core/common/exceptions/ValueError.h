#ifndef VTCPD_VALUEERROR_H
#define VTCPD_VALUEERROR_H

#include "Exception.h"


class ValueError: public Exception
{
    using Exception::Exception;
};


#endif //VTCPD_VALUEERROR_H
