#ifndef _NETWORK_LEVEL3_MM_MESSAGE_H
#define _NETWORK_LEVEL3_MM_MESSAGE_H

#include "Network/GsmMessage.h"

class MmMessage : public GsmMessage
{
public:
    void parse(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> pack() const override;
    virtual uint8_t messageType() const = 0;

protected:
    MmMessage();
};

#endif
