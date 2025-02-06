#ifndef VTCPD_PATHSRESOURCE_H
#define VTCPD_PATHSRESOURCE_H

#include "BaseResource.h"
#include "../../paths/lib/PathsCollection.h"

class PathsResource : public BaseResource
{

public:
    typedef shared_ptr<PathsResource> Shared;

public:
    PathsResource(
        const TransactionUUID &transactionUUID,
        PathsCollection::Shared pathsCollection);

    PathsCollection::Shared pathCollection() const;

private:
    PathsCollection::Shared mPathsCollection;
};


#endif //VTCPD_PATHSRESOURCE_H
