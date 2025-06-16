#ifndef _NETWORK_BCD_H
#define _NETWORK_BCD_H

#include <vector>
#include <cstdint>
#include <string>

class Bcd
{
public:
    static std::string parse(const std::vector<uint8_t>& bcdData);
    static std::vector<uint8_t> pack(const std::string& digits);
};

#endif
