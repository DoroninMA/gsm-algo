#include <stdexcept>
#include <Network/Level3/MmMessage/LocationUpdateReject.h>

static constexpr uint8_t TLV_CAUSE_TAG = 0x08;

LocationUpdateReject::LocationUpdateReject(uint8_t cause): LocationUpdateReject()
{
    setCause(cause);
}

uint8_t LocationUpdateReject::cause() const
{
    if (_cause.length() != 1)
    {
        throw std::runtime_error("LocationUpdateReject: Invalid cause length");
    }

    return _cause.value().at(0);
}

uint8_t LocationUpdateReject::messageType() const
{
    return static_cast<uint8_t>(GsmMsgTypeMM::LOCATION_UPDATE_REJECT);
}

std::vector<uint8_t> LocationUpdateReject::pack() const
{
    std::vector<uint8_t> out = MmMessage::pack();
    std::vector<uint8_t> causeEnc = _cause.pack();
    out.insert(out.end(), causeEnc.begin(), causeEnc.end());
    return out;
}

void LocationUpdateReject::parse(const std::vector<uint8_t>& data)
{
    if (data.size() < 4)
    {
        throw std::runtime_error("LocationUpdateReject: Invalid size");
    }

    MmMessage::parse(data);

    size_t offset = 2;
    Tlv causeTlv = Tlv::parse(data, offset);
    if ((causeTlv.tag() != TLV_CAUSE_TAG) || (causeTlv.length() != 1))
    {
        throw std::runtime_error("LocationUpdateReject: Invalid Cause IE");
    }

    _cause = causeTlv;
}

void LocationUpdateReject::setCause(uint8_t cause)
{
    _cause = Tlv(TLV_CAUSE_TAG, std::vector<uint8_t>{cause});
}
