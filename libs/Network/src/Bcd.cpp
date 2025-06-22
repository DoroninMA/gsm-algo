#include <Network/Bcd.h>

#include <stdexcept>


std::vector<uint8_t> Bcd::pack(const std::string& digits)
{
    std::vector<uint8_t> bcd;
    for (size_t i = 0; i < digits.size(); i += 2)
    {
        uint8_t low = digits[i] - '0';
        // uint8_t low = digits[i];
        if (low > 9)
        {
            throw std::invalid_argument("Invalid digit");
        }

        uint8_t high = 0xF;
        if (i + 1 < digits.size())
        {
            high = digits[i + 1] - '0';
            // high = digits[i + 1];
            if (high > 9)
            {
                throw std::invalid_argument("Invalid digit");
            }
        }

        bcd.push_back((low & 0x0F) | ((high & 0x0F) << 4));
    }
    return bcd;
}

std::string Bcd::parse(const std::vector<uint8_t>& bcdData)
{
    std::string result;
    for (uint8_t byte: bcdData)
    {
        uint8_t low = byte & 0x0F;
        uint8_t high = (byte >> 4) & 0x0F;
        if (low < 0xA)
        {
            result += ('0' + low);
        }
        if (high < 0xA)
        {
            result += ('0' + high);
        }
    }
    return result;
}
