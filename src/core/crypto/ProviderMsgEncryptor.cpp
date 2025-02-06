#include "ProviderMsgEncryptor.h"

ProviderMsgEncryptor::ProviderMsgEncryptor(
    const ProviderMsgEncryptor::PublicKey::Shared &publicKey) : mPublicKey(publicKey)
{
}

ProviderMsgEncryptor::Buffer ProviderMsgEncryptor::encrypt(
    byte_t* bytesForEncryption,
    size_t size)
{
    size_t additionalDataSize = 7;
    byte_t additionalData[] = "A256GCM";

    if (crypto_aead_aes256gcm_is_available() == 0) {
        return make_pair(nullptr, 0);
    }

    byte_t nonce[crypto_aead_aes256gcm_NPUBBYTES];
    randombytes_buf(nonce, sizeof nonce);

    byte_t encryptedData[size + crypto_aead_aes256gcm_ABYTES];
    unsigned long long encryptedDataSize;
    crypto_aead_aes256gcm_encrypt(
        encryptedData,
        &encryptedDataSize,
        bytesForEncryption,
        size,
        additionalData,
        additionalDataSize,
        nullptr,
        nonce,
        mPublicKey->key);

    auto encryptedDataBuffer = tryMalloc(
                                   encryptedDataSize);
    memcpy(
        encryptedDataBuffer.get(),
        encryptedData,
        encryptedDataSize);

    return make_pair(
               encryptedDataBuffer,
               encryptedDataSize);
}

static void parseHex(
    byte_t* out,
    const string &in)
{
    char b[3], i = 0;
    b[2] = '\0';
    for (const char* p = in.c_str(), *e = p + in.length(); p < e; p += 2, ++i) {
        memcpy(b, p, 2);
        out[i] = (byte_t)std::stoul(b, nullptr, 16);
    }
}

ProviderMsgEncryptor::PublicKey::PublicKey(
    const string &str)
{
    // todo : check if str has valid length
    parseHex(key, str);
}