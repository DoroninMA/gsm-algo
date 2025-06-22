#include <Network/Level3/MmMessage/MmMessage.h>

#include <stdexcept>

GsmMessagePD MmMessage::protocolDiscriminator() const
{
    return GsmMessagePD::MOBILITY_MANAGEMENT;
}

void MmMessage::parse(const std::vector<uint8_t>& data)
{
    if (data.size() < sizeof(protocolDiscriminator()) ||
        ((data[0] >> 4) != static_cast<uint8_t>(protocolDiscriminator())))
    {
        throw std::runtime_error("MmMessage::parse(): invalid message protocol discriminator");
    }
}

std::vector<uint8_t> MmMessage::pack() const
{
    uint8_t pd = static_cast<uint8_t>(protocolDiscriminator());
    pd = (pd << 4) & 0xF0;
    return { pd, messageType() };
}
