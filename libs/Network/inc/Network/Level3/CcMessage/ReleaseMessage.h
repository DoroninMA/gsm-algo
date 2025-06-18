#ifndef _NETWORK_LEVEL3_СС_MESSAGE_RELEASE_MESSAGE_H
#define _NETWORK_LEVEL3_СС_MESSAGE_RELEASE_MESSAGE_H

#include "CcMessage.h"
#include <Network/Tlv.h>

class ReleaseMessage : public CcMessage
{
public:
    ReleaseMessage();
    explicit ReleaseMessage(uint8_t cause);

    uint8_t messageType() const override;
    uint8_t cause() const;

    std::vector<uint8_t> pack() const override;
    void parse(const std::vector<uint8_t>& data) override;
    void setCause(uint8_t cause);

private:
    Tlv _cause;
};

#endif
