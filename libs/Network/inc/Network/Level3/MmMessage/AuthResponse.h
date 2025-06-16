#ifndef _NETWORK_LEVEL3_AUTH_RESPONSE_MESSAGE_H
#define _NETWORK_LEVEL3_AUTH_RESPONSE_MESSAGE_H

#include <Network/Tlv.h>
#include <Network/GsmMessage.h>
#include <vector>
#include <cstdint>


class AuthResponseMessage final : public GsmMessage
{
public:
    AuthResponseMessage();

    std::vector<uint8_t> sres() const;
    void setSres(const std::vector<uint8_t>& sresData);

    void parse(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> pack() const override;
    uint8_t getMessageType() const override;

private:
    Tlv _sres;
};


#endif
