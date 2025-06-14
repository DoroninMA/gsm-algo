#ifndef _NETWORK_TLV_H
#define _NETWORK_TLV_H

#include <vector>
#include <cstdint>

class Tlv
{
public:
    Tlv();
    Tlv(uint8_t tag, const std::vector<uint8_t>& value);
    Tlv(const Tlv& other);

    uint8_t tag() const;
    uint8_t length() const;
    std::vector<uint8_t> value() const;

    std::vector<uint8_t> pack() const;
    static Tlv parse(const std::vector<uint8_t>& data, size_t offset=0);

private:
    uint8_t _tag;
    std::vector<uint8_t> _value;
};

#endif
