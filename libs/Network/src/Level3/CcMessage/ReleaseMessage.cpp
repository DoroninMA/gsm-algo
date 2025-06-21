#include <Network/Level3/CcMessage/ReleaseMessage.h>

#include <stdexcept>

static constexpr uint8_t TLV_CAUSE_TAG = 0x08;

ReleaseMessage::ReleaseMessage()
{
    setCause(0);
}

ReleaseMessage::ReleaseMessage(uint8_t cause)
{
    setCause(cause);
}

uint8_t ReleaseMessage::messageType() const
{
    return static_cast<uint8_t>(GsmMsgTypeCC::RELEASE);
}

uint8_t ReleaseMessage::cause() const
{
    if (_cause.length() != 1)
    {
        throw std::runtime_error("ReleaseMessage: invalid cause length");
    }

    return _cause.value().at(0);
}

std::vector<uint8_t> ReleaseMessage::pack() const
{
    std::vector<uint8_t> out = CcMessage::pack();
    std::vector<uint8_t> causeEnc = _cause.pack();
    out.insert(out.end(), causeEnc.cbegin(), causeEnc.cend());
    return out;
}

void ReleaseMessage::parse(const std::vector<uint8_t>& data)
{
    CcMessage::parse(data);

    size_t offset = 1;
    Tlv causeTlv = Tlv::parse(data, offset);
    if (causeTlv.tag() != TLV_CAUSE_TAG)
    {
        throw std::runtime_error("ReleaseMessage: expected Cause IE");
    }

    if (causeTlv.length() != 1)
    {
        throw std::runtime_error("ReleaseMessage: Cause IE length invalid");
    }

    _cause = causeTlv;
}

void ReleaseMessage::setCause(uint8_t cause)
{
    _cause = Tlv(TLV_CAUSE_TAG, std::vector<uint8_t>{cause});
}
