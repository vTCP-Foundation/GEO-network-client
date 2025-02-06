#ifndef LAMPORTKEYS_H
#define LAMPORTKEYS_H

#include "memory.h"

#include <sodium.h>
#include <memory>

namespace crypto {
namespace lamport {

using namespace std;

class BaseKey : boost::noncopyable
{
    friend class Signature;

public:
    static const size_t keySize();

protected:
    static const size_t kRandomNumbersCount = 256 * 2;
    static const size_t kRandomNumberSize = 256 / 8;
};

class KeyHash
{
public:
    typedef shared_ptr<KeyHash> Shared;

public:
    KeyHash() = default;

    KeyHash(
        byte_t* buffer);

    const byte_t* data() const;

    const string toString() const;

    friend bool operator==(
        const KeyHash &kh1,
        const KeyHash &kh2);

    friend bool operator!=(
        const KeyHash &kh1,
        const KeyHash &kh2);

public:
    static const size_t kBytesSize = 32;

private:
    byte_t mData[kBytesSize];
};

class PublicKey : public BaseKey
{
    friend class PrivateKey;
    friend class Signature;

public:
    typedef shared_ptr<PublicKey> Shared;

    PublicKey() = default;

    PublicKey(
        byte_t* data);

    ~PublicKey() noexcept;

    const byte_t* data() const;

    const KeyHash::Shared hash() const;

public:
    using BaseKey::BaseKey;

private:
    byte_t* mData;
};

class PrivateKey : public BaseKey
{
    friend class Signature;

public:
    explicit PrivateKey();

    PrivateKey(
        byte_t* data);

    PublicKey::Shared derivePublicKey();

    void crop();

    const memory::SecureSegment *data() const;

private:
    memory::SecureSegment mData;
    bool mIsCropped;
};

}
}

#endif // LAMPORTKEYS_H
