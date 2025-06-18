#include <Network/Level3/MmMessage/CipherModeComplete.h>

#include <stdexcept>

static constexpr uint8_t TLV_MI_TAG = 0x22; // Mobile Identity


CipherModeComplete::CipherModeComplete() : _isMobileIdentityExist(false)
{
}

uint8_t CipherModeComplete::messageType() const
{
    return static_cast<uint8_t>(GsmMsgTypeL3::CIPHER_MODE_COMMAND);
}

bool CipherModeComplete::isMobileIdentityExist() const
{
    return _isMobileIdentityExist;
}

std::vector<uint8_t> CipherModeComplete::mobileIdentity() const
{
    if (_isMobileIdentityExist)
    {
        throw std::runtime_error("CipherModeComplete: wrong mobile identity");
    }

    return _mobileIdentity.value();
}

void CipherModeComplete::parse(const std::vector<uint8_t>& data)
{
    size_t offset = 0;
    MmMessage::parse(data);
    offset++;

    if (offset >= data.size())
    {
        throw std::runtime_error("CipherModeComplete: missing message type");
    }

    uint8_t msgType = data[offset++];
    if (msgType != messageType())
    {
        throw std::runtime_error("CipherModeComplete: wrong message type");
    }

    if (offset < data.size())
    {
        Tlv mi = Tlv::parse(data, offset);
        if (mi.tag() != TLV_MOBILE_IDENTITY_TAG)
        {
            throw std::runtime_error("CipherModeComplete: invalid mobile identity tag");
        }

        _mobileIdentity = mi;
        _isMobileIdentityExist = true;
    }
    else
    {
        _isMobileIdentityExist = false;
    }
}

std::vector<uint8_t> CipherModeComplete::pack() const
{
    std::vector<uint8_t> out = MmMessage::pack();
    out.push_back(messageType());

    if (_isMobileIdentityExist)
    {
        std::vector<uint8_t> miPacked = _mobileIdentity.pack();
        out.insert(out.end(), miPacked.begin(), miPacked.end());
    }

    return out;
}

void CipherModeComplete::setMobileIdentity(const std::vector<uint8_t>& mi)
{
    if (mi.empty())
    {
        throw std::invalid_argument("CipherModeCommand: Mobile Identity cannot be empty");
    }

    _mobileIdentity = Tlv(TLV_MI_TAG, mi);
    _isMobileIdentityExist = true;
}
