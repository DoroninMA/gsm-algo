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

    _sres = Tlv(TLV_KC_TAG, sresData);
}

void AuthResponseMessage::parse(const std::vector<uint8_t>& data)
{
    MmMessage::parse(data);
    size_t offset = 1;

    if (data.size() <= offset)
    {
        throw std::runtime_error("AuthResponseMessage: data too short for message type");
    }

    uint8_t msgType = data[offset++];
    if (msgType != messageType())
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

    std::vector<uint8_t> out = MmMessage::pack();
    out.push_back(messageType());

    std::vector<uint8_t> sresEncoded = _sres.pack();
    out.insert(out.end(), sresEncoded.begin(), sresEncoded.end());

    return out;
}

uint8_t AuthResponseMessage::messageType() const
{
    return static_cast<uint8_t>(GsmMsgTypeL3::AUTH_RESPONSE);
}
