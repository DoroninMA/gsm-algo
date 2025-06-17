#include <Network/Level3/MmMessage/MmMessage.h>

#include <stdexcept>

MmMessage::MmMessage()
{
    _pd = GsmMessagePD::MOBILITY_MANAGEMENT;
}

void MmMessage::parse(const std::vector<uint8_t>& data)
{
    if (data.size() < sizeof(_pd) || data[0] != static_cast<uint8_t>(_pd))
    {
        throw std::runtime_error("MmMessage::parse(): invalid message protocol discriminator");
    }
}

std::vector<uint8_t> MmMessage::pack() const
{
    return { static_cast<uint8_t>(_pd) };
}
