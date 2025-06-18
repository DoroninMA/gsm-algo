#ifndef _NETWORK_LEVEL3_СС_MESSAGE_CONNECT_ACK_H
#define _NETWORK_LEVEL3_СС_MESSAGE_CONNECT_ACK_H

#include "CcMessage.h"

class ConnectAck final : public CcMessage
{
public:
    ConnectAck() = default;

    uint8_t messageType() const override;
    std::vector<uint8_t> pack() const override;
    void parse(const std::vector<uint8_t>& data) override;
};

#endif
