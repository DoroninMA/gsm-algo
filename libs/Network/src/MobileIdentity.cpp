#include <Network/MobileIdentity.h>
#include <Network/Bcd.h>

#include <stdexcept>


void MobileIdentity::parse(const std::vector<uint8_t>& data)
{
    if (data.empty())
    {
        throw std::runtime_error("Empty Mobile Identity");
    }

    setType(static_cast<Type>(data[0] & 0b111)); // type is only lower 3 bits

    if ((_type == Type::IMSI) || (_type == Type::IMEI))
    {
        setValue(std::vector<uint8_t>(data.begin() + 1, data.end()));
    }
    else if (_type == Type::TMSI)
    {
        if (data.size() != 5)
        {
            throw std::runtime_error("TMSI must be 5 bytes including type");
        }
        setValue(std::vector<uint8_t>(data.begin() + 1, data.end()));
    }
    else
    {
        throw std::runtime_error("Unsupported Mobile Identity type");
    }
}

std::vector<uint8_t> MobileIdentity::pack() const
{
    std::vector<uint8_t> result;

    if ((_type == Type::IMSI) || (_type == Type::IMEI))
    {
        const uint8_t header = (static_cast<uint8_t>(_type) & 0x07);
        result.push_back(header);
        result.insert(result.end(), _value.begin(), _value.end());
    }
    else if (_type == Type::TMSI)
    {
        if (_value.size() != 4)
        {
            throw std::runtime_error("TMSI must be 4 bytes");
        }

        result.push_back(0xF0 | (static_cast<uint8_t>(_type) & 0x07));
        result.insert(result.end(), _value.cbegin(), _value.cend());
    }

    return result;
}

std::string MobileIdentity::toString() const
{
    if ((_type == Type::IMSI) || (_type == Type::IMEI))
    {
        return Bcd::parse(_value);
    }

    return "";
}

MobileIdentity MobileIdentity::fromString(Type type, const std::string& str)
{
    if ((type != Type::IMSI) && (type != Type::IMEI))
    {
        throw std::runtime_error("Only IMSI and IMEI supported in fromString");
    }

    MobileIdentity id;
    id.setType(type);
    id.setValue(Bcd::pack(str));
    return id;
}

void MobileIdentity::setType(Type t)
{
    _type = t;
}

MobileIdentity::Type MobileIdentity::getType() const
{
    return _type;
}

void MobileIdentity::setValue(const std::vector<uint8_t>& v)
{
    _value = v;
}

const std::vector<uint8_t>& MobileIdentity::getValue() const
{
    return _value;
}
