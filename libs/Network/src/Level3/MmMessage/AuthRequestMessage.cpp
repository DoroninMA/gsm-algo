#include <Network/Level3/MmMessage/AuthRequestMessage.h>

#include <stdexcept>
#include "Network/GsmDefs.h"

static constexpr uint8_t TLV_RAND_TAG = 0x21;
static constexpr uint8_t TLV_KC_TAG = 0x22;

AuthRequestMessage::AuthRequestMessage()
    : _isKcExist(false)
{
    setRand(std::vector<uint8_t>(16, 0));
}

AuthRequestMessage::AuthRequestMessage(const std::vector<uint8_t>& rand, const std::vector<uint8_t>& kc)
{
    setRand(rand);
    setKc(kc);
}

std::vector<uint8_t> AuthRequestMessage::kc() const
{
    if (!_isKcExist)
    {
        throw std::runtime_error("AuthRequestMessage: Kc is not exist");
    }

    return _kc.value();
}

std::vector<uint8_t> AuthRequestMessage::rand() const
{
    return _rand.value();
}

uint8_t AuthRequestMessage::messageType() const
{
    return static_cast<uint8_t>(GsmMsgTypeMM::AUTH_REQUEST);
}

void AuthRequestMessage::parse(const std::vector<uint8_t>& data)
{
    size_t offset = 0;

    MmMessage::parse(data);
    offset += 1;

    if (offset >= data.size())
    {
        throw std::runtime_error("AuthRequestMessage: data too short for message type");
    }

    const uint8_t msgType = data[offset++];
    if (msgType != messageType())
    {
        throw std::runtime_error("AuthRequestMessage: incorrect message type");
    }

    if (offset >= data.size())
    {
        throw std::runtime_error("AuthRequestMessage: missing RAND TLV");
    }

    _rand = Tlv::parse(data, offset);
    if (_rand.tag() != TLV_RAND_TAG || _rand.length() != 16)
    {
        throw std::runtime_error("AuthRequestMessage: invalid RAND TLV");
    }

    if (offset < data.size())
    {
        _kc = Tlv::parse(data, offset);
        if (_kc.tag() != TLV_KC_TAG || _kc.length() != 8)
        {
            throw std::runtime_error("AuthRequestMessage: invalid Kc TLV");
        }

        _isKcExist = true;
    }
    else
    {
        _isKcExist = false;
    }
}

void AuthRequestMessage::setKc(const std::vector<uint8_t>& kc)
{
    _kc = Tlv(TLV_KC_TAG, kc);
    _isKcExist = true;
}

void AuthRequestMessage::setRand(const std::vector<uint8_t>& rand)
{
    _rand = Tlv(TLV_RAND_TAG, rand);
}

std::vector<uint8_t> AuthRequestMessage::pack() const
{
    std::vector<uint8_t> out = MmMessage::pack();
    out.push_back(messageType());

    if (_rand.tag() != TLV_RAND_TAG || _rand.length() != 16)
    {
        throw std::runtime_error("AuthRequestMessage: RAND must be 16 bytes");
    }

    std::vector<uint8_t> randEncoded = _rand.pack();
    out.insert(out.end(), randEncoded.begin(), randEncoded.end());

    if (_isKcExist)
    {
        if (_kc.tag() != TLV_KC_TAG || _kc.length() != 8)
        {
            throw std::runtime_error("AuthRequestMessage: Kc must be 8 bytes");
        }

        std::vector<uint8_t> kcEncoded = _kc.pack();
        out.insert(out.end(), kcEncoded.begin(), kcEncoded.end());
    }

    return out;
}
