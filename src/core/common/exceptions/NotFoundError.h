#ifndef VTCPD_NOTFOUNDERROR_H
#define VTCPD_NOTFOUNDERROR_H

#include "Exception.h"


class NotFoundError: public Exception
{
    using Exception::Exception;
};


#endif //VTCPD_NOTFOUNDERROR_H
