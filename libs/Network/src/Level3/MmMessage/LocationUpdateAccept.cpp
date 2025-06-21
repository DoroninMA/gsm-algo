#include <Network/Level3/MmMessage/LocationUpdateAccept.h>

#include <stdexcept>

LocationUpdateAccept::LocationUpdateAccept(const std::vector<uint8_t>& lai)
{
    setLai(lai);
}

void LocationUpdateAccept::setLai(const std::vector<uint8_t>& lai)
{
    if (lai.size() != 5)
    {
        throw std::runtime_error("LocationUpdateAccept: LAI must be 5 bytes");
    }

    _lai = lai;
}

const std::vector<uint8_t>& LocationUpdateAccept::lai() const
{
    return _lai;
}

uint8_t LocationUpdateAccept::messageType() const
{
    return static_cast<uint8_t>(GsmMsgTypeMM::LOCATION_UPDATE_ACCEPT);
}

std::vector<uint8_t> LocationUpdateAccept::pack() const
{
    std::vector<uint8_t> out = MmMessage::pack();
    out.insert(out.end(), _lai.cbegin(), _lai.cend());
    return out;
}

void LocationUpdateAccept::parse(const std::vector<uint8_t>& data)
{
    if (data.size() < 7)
    {
        throw std::runtime_error("LocationUpdateAccept: invalid size");
    }

    MmMessage::parse(data);
    _lai.assign(data.begin() + 2, data.begin() + 7);
}
