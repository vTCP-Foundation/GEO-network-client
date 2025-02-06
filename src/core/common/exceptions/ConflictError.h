#ifndef VTCPD_CONFLICTERROR_H
#define VTCPD_CONFLICTERROR_H

#include "Exception.h"


class ConflictError: public Exception
{
    using Exception::Exception;
};

#endif //VTCPD_CONFLICTERROR_H
