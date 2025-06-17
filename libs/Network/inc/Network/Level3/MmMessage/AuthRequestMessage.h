#ifndef _NETWORK_LEVEL3_MM_MESSAGE_AUTH_REQUEST_MESSAGE_H
#define _NETWORK_LEVEL3_MM_MESSAGE_AUTH_REQUEST_MESSAGE_H

#include <Network/Tlv.h>
#include <Network/GsmMessage.h>

#include "MmMessage.h"

class AuthRequestMessage final : public MmMessage
{
public:
    AuthRequestMessage();
    AuthRequestMessage(const std::vector<uint8_t>& rand, const std::vector<uint8_t>& kc);

    std::vector<uint8_t> kc() const;
    std::vector<uint8_t> rand() const;
    uint8_t messageType() const override;

    std::vector<uint8_t> pack() const override;
    void parse(const std::vector<uint8_t>& data) override;

    void setKc(const std::vector<uint8_t>& kc);
    void setRand(const std::vector<uint8_t>& rand);

private:
    Tlv _kc;
    Tlv _rand;
    bool _isKcExist;
};

#endif
