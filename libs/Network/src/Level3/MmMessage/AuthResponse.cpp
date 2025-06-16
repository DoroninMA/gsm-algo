#include <Network/Level3/MmMessage/AuthResponse.h>

#include <stdexcept>
#include "Network/GsmDefs.h"

static constexpr uint8_t TLV_KC_TAG = 0x22;

AuthResponseMessage::AuthResponseMessage()
    : _sres(TLV_KC_TAG, std::vector<uint8_t>(4, 0))
{
}

std::vector<uint8_t> AuthResponseMessage::sres() const
{
    return _sres.value();
}

void AuthResponseMessage::setSres(const std::vector<uint8_t>& sresData)
{
    if (sresData.size() != 4)
    {
        throw std::invalid_argument("AuthResponseMessage: SRES must be exactly 4 bytes");
    }

    _sres = Tlv(0x22, sresData);
}

void AuthResponseMessage::parse(const std::vector<uint8_t>& data)
{
    size_t offset = 0;

    if (data.size() < 1)
    {
        throw std::runtime_error("AuthResponseMessage: data too short");
    }

    uint8_t msgType = data[offset++];
    if (msgType != getMessageType())
    {
        throw std::runtime_error("AuthResponseMessage: incorrect message type");
    }

    _sres = Tlv::parse(data, offset);
    if (_sres.tag() != TLV_KC_TAG || _sres.length() != 4)
    {
        throw std::runtime_error("AuthResponseMessage: invalid SRES TLV");
    }
}

std::vector<uint8_t> AuthResponseMessage::pack() const
{
    if (_sres.tag() != TLV_KC_TAG || _sres.length() != 4)
    {
        throw std::runtime_error("AuthResponseMessage: SRES must be 4 bytes");
    }

    std::vector<uint8_t> out;
    out.push_back(getMessageType());

    std::vector<uint8_t> sresEncoded = _sres.pack();
    out.insert(out.end(), sresEncoded.begin(), sresEncoded.end());

    return out;
}

uint8_t AuthResponseMessage::getMessageType() const
{
    return static_cast<uint8_t>(GsmMsgTypeL3::AUTH_RESPONSE);
}
