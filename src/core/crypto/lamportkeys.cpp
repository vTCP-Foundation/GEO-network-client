#include "lamportkeys.h"

namespace crypto {
namespace lamport {

const size_t BaseKey::keySize()
{
    // public and private keys has 16KB
    return 16 * 1024;
}

PrivateKey::PrivateKey() : mData(memory::SecureSegment(kRandomNumbersCount * kRandomNumberSize)),
    mIsCropped(false)
{
    auto guard = mData.unlockAndInitGuard();

    auto offset = static_cast<byte_t*>(guard.address());
    for (size_t i = 0; i < kRandomNumbersCount; ++i) {
        randombytes_buf(offset, kRandomNumberSize);
        offset += kRandomNumberSize;
    }
}

PrivateKey::PrivateKey(
    byte_t* data) : mData(memory::SecureSegment(keySize())),
    mIsCropped(false)
{
    auto guard = mData.unlockAndInitGuard();
    auto offset = static_cast<byte_t*>(guard.address());
    memcpy(
        offset,
        data,
        keySize());
}

PublicKey::Shared PrivateKey::derivePublicKey()
{

    auto guard = mData.unlockAndInitGuard();
    auto generatedKey = make_shared<PublicKey>();

    // Numbers buffers memory allocation.
    generatedKey->mData = static_cast<byte_t*>(malloc(kRandomNumbersCount * kRandomNumberSize));
    if (generatedKey->mData == nullptr) {
        return nullptr;
    }

    // Numbers buffers initialisation via hashing private key numbers.
    auto source = static_cast<byte_t*>(guard.address());
    auto destination = static_cast<byte_t*>(generatedKey->mData);

    for (size_t i = 0; i < kRandomNumbersCount; ++i) {
        crypto_generichash(destination, kRandomNumberSize, source, kRandomNumberSize, nullptr, 0);
        source += kRandomNumberSize;
        destination += kRandomNumberSize;
    }

    return generatedKey;
}

const memory::SecureSegment *PrivateKey::data() const
{
    return &mData;
}

PublicKey::PublicKey(
    byte_t* data)
{
    mData = static_cast<byte_t*>(
                malloc(
                    keySize()));
    memcpy(
        mData,
        data,
        keySize());
}

PublicKey::~PublicKey() noexcept
{
    if (mData != nullptr) {
        free(mData);
        mData = nullptr;
    }
}

const byte_t* PublicKey::data() const
{
    return mData;
}

const KeyHash::Shared PublicKey::hash() const
{
    auto keyHashBuffer = (byte_t*)malloc(KeyHash::kBytesSize);
    crypto_generichash(
        keyHashBuffer,
        KeyHash::kBytesSize,
        mData,
        keySize(),
        nullptr,
        0);
    return make_shared<KeyHash>(keyHashBuffer);
}

KeyHash::KeyHash(
    byte_t* buffer)
{
    memcpy(
        mData,
        buffer,
        kBytesSize);
}

const byte_t* KeyHash::data() const
{
    return mData;
}

const string KeyHash::toString() const
{
    stringstream ss;
    ss << std::hex;
    for (byte_t i : mData)
        ss << (int)i;
    return ss.str();
}

bool operator==(const KeyHash &kh1, const KeyHash &kh2)
{
    for (int i = KeyHash::kBytesSize - 1; i >= 0; --i) {
        if (kh1.mData[i] != kh2.mData[i])
            return false;
    }
    return true;
}

bool operator!=(const KeyHash &kh1, const KeyHash &kh2)
{
    for (int i = KeyHash::kBytesSize - 1; i >= 0; --i) {
        if (kh1.mData[i] != kh2.mData[i])
            return true;
    }
    return false;
}

}
}
