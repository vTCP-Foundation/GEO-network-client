#ifndef VTCPD_MSGENCRYPTOR_H
#define VTCPD_MSGENCRYPTOR_H

#include "ByteEncryptor.h"

class Message;

class MsgEncryptor : public ByteEncryptor
{
public:
    typedef shared_ptr<Message> MessageShared;

    struct KeyTrio : KeyPair
    {
        typedef std::shared_ptr<KeyTrio> Shared;
        KeyTrio() = default;
        explicit KeyTrio(const string &str);
        explicit KeyTrio(vector<byte_t> &in);
        void serialize(vector<byte_t> &out) const;
        void deserialize(vector<byte_t> &in);
        PublicKey::Shared contractorPublicKey = nullptr;
    };

public:
    using ByteEncryptor::ByteEncryptor;

public:
    static KeyTrio::Shared generateKeyTrio(
        const string &contractorPublicKey = "");

    static KeyTrio::Shared generateKeyTrio(
        PublicKey::Shared contractorPublicKey);

public:
    Buffer encrypt(MessageShared message);
    Buffer decrypt(
        BytesShared buffer,
        const size_t count);
};

std::ostream &operator<<(std::ostream &out, const MsgEncryptor::KeyTrio &t);

#endif // VTCPD_MSGENCRYPTOR_H
