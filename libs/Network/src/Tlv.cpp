#include <Network/Tlv.h>

#include <stdexcept>

Tlv::Tlv() : _tag(0), _value()
{
}

Tlv::Tlv(uint8_t tag, const std::vector<uint8_t>& value)
    : _tag(tag), _value(value)
{
}

Tlv::Tlv(const Tlv& other) : _tag(other._tag), _value(other._value)
{
}

uint8_t Tlv::tag() const
{
    return _tag;
}

uint8_t Tlv::length() const
{
    return static_cast<uint8_t>(_value.size());
}

std::vector<uint8_t> Tlv::value() const
{
    return _value;
}

std::vector<uint8_t> Tlv::pack() const
{
    if (_value.size() > 255)
    {
        throw std::runtime_error("TLV value too long");
    }

    std::vector<uint8_t> result;
    result.reserve( sizeof(_tag) + 1 + _value.size());

    result.push_back(_tag);
    result.push_back(static_cast<uint8_t>(_value.size()));
    result.insert(result.end(), _value.cbegin(), _value.cend());
    return result;
}

Tlv Tlv::parse(const std::vector<uint8_t>& data, size_t offset)
{
    if (offset + 2 > data.size())
    {
        throw std::runtime_error("TLV data out of bounds");
    }

    uint8_t tag = data[offset++];
    uint8_t len = data[offset++];

    if (offset + len > data.size())
    {
        throw std::runtime_error("TLV decode: value out of range");
    }

    std::vector<uint8_t> val(data.begin() + offset, data.begin() + offset + len);
    offset += len;

    return Tlv(tag, val);
}
