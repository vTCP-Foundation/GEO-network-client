#ifndef VTCPD_OVERFLOWERROR_H
#define VTCPD_OVERFLOWERROR_H

#include "Exception.h"


class OverflowError: public Exception
{
    using Exception::Exception;
};

#endif //VTCPD_OVERFLOWERROR_H
