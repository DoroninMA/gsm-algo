#ifndef _NETWORK_LEVEL3_MM_MESSAGE_AUTH_RESPONSE_MESSAGE_H
#define _NETWORK_LEVEL3_MM_MESSAGE_AUTH_RESPONSE_MESSAGE_H

#include <Network/Tlv.h>
#include <Network/GsmMessage.h>

#include "MmMessage.h"


class AuthResponseMessage final : public MmMessage
{
public:
    AuthResponseMessage();

    std::vector<uint8_t> sres() const;
    void setSres(const std::vector<uint8_t>& sresData);

    void parse(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> pack() const override;
    uint8_t messageType() const override;

private:
    Tlv _sres;
};


#endif
