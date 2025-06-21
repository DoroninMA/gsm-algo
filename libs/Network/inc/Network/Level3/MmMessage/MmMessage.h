#ifndef _NETWORK_LEVEL3_MM_MESSAGE_H
#define _NETWORK_LEVEL3_MM_MESSAGE_H

#include "Network/GsmMessage.h"

class MmMessage : public GsmMessage
{
public:
    GsmMessagePD protocolDiscriminator() const final;

    void parse(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> pack() const override;

protected:
    MmMessage();
};

#endif
