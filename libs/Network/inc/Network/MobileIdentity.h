#ifndef _NETWORK_MOBILE_IDENTITY_H
#define _NETWORK_MOBILE_IDENTITY_H

#include <vector>
#include <cstdint>
#include <string>

class MobileIdentity {
public:
    enum class Type : uint8_t {
        IMSI = 0x01,
        IMEI = 0x02,
        TMSI = 0x04
    };

    Type getType() const;
    const std::vector<uint8_t>& getValue() const;

    std::vector<uint8_t> pack() const;
    std::string toString() const;

    void setType(Type t);
    void setValue(const std::vector<uint8_t>& v);

    void parse(const std::vector<uint8_t>& data);
    static MobileIdentity fromString(Type type, const std::string& str);

private:
    Type _type = Type::IMSI;
    std::vector<uint8_t> _value;
};

#endif
