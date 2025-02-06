#ifndef VTCPD_EQUIVALENTMESSAGE_H
#define VTCPD_EQUIVALENTMESSAGE_H

#include "Message.hpp"

/*
 * Abstract base class for messages that must contain equivalent.
 */
class EquivalentMessage : public Message
{

public:
    EquivalentMessage(
        const SerializedEquivalent equivalent);

    EquivalentMessage(
        BytesShared buffer);

    const SerializedEquivalent equivalent() const;

    virtual pair<BytesShared, size_t> serializeToBytes() const override;

protected:
    virtual const size_t kOffsetToInheritedBytes() const override;

private:
    SerializedEquivalent mEquivalent;
};


#endif //VTCPD_EQUIVALENTMESSAGE_H
