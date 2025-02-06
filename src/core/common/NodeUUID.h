#ifndef GEO_NETWORK_CLIENT_NODEUUID_H
#define GEO_NETWORK_CLIENT_NODEUUID_H


#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/endian/arithmetic.hpp>

#include <string>


using namespace boost::uuids;
using namespace std;


class NodeUUID:
    public uuid
{

public:
    static const size_t kHexSize = 36;
    static const size_t kBytesSize = 16;

    static const NodeUUID& empty();

public:
    explicit NodeUUID();
    NodeUUID(uuid const &u);
    NodeUUID(NodeUUID &u);
    NodeUUID(const NodeUUID &u);
    NodeUUID(const string &hex);
    explicit NodeUUID(const uint8_t* bytes);

    NodeUUID& operator=(
        const boost::uuids::uuid &u);

    const string stringUUID() const;
};

#endif //GEO_NETWORK_CLIENT_NODEUUID_H
