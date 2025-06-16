#include <Network/Bcd.h>

#include <stdexcept>


std::string Bcd::parse(const std::vector<uint8_t>& bcdData)
{
    std::string result;

    for (uint8_t byte : bcdData) {
        uint8_t low = byte & 0x0F;
        uint8_t high = (byte >> 4) & 0x0F;

        if (low < 0xA) result += ('0' + low);
        if (high < 0xA) result += ('0' + high);
    }

    return result;
}

std::vector<uint8_t> Bcd::pack(const std::string& digits)
{
    std::vector<uint8_t> bcd;
    size_t len = digits.size();

    for (size_t i = 0; i < len; i += 2)
    {
        uint8_t low = digits[i] - '0';
        if (low > 9)
        {
            throw std::invalid_argument("Invalid digit");
        }

        uint8_t high = 0xF;
        if (i + 1 < len)
        {
            high = digits[i + 1] - '0';
            if (high > 9)
            {
                throw std::invalid_argument("Invalid digit");
            }
        }

        bcd.push_back((high << 4) | low);
    }

    return bcd;
}
