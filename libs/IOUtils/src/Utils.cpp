#include <algorithm>
#include <stdexcept>

#include <IOUtils/Utils.h>

std::string bytesToHexString(const uint8_t* str, size_t bytes)
{
    static const char hexChars[] = "0123456789ABCDEF";
    std::string result;
    result.reserve(bytes * 2);

    for (size_t i = 0; i < bytes; ++i)
    {
        uint8_t byte = str[i];
        result.push_back(hexChars[byte >> 4]);
        result.push_back(hexChars[byte & 0x0F]);
    }

    return result;
}

std::string bytesToBitString(const uint8_t* str, size_t bytes)
{
    std::string result;
    result.reserve(bytes * 8);

    for (size_t i = 0; i < bytes; ++i)
    {
        uint8_t byte = str[i];
        for (int j = 7; j >= 0; --j)
        {
            result.push_back((byte & (1 << j)) ? '1' : '0');
        }
    }

    return result;
}

static inline uint8_t hexToByte(char hexSymbol)
{
    hexSymbol = tolower(hexSymbol);

    if (hexSymbol >= '0' && hexSymbol <= '9')
    {
        return hexSymbol - '0';
    }
    else if (hexSymbol >= 'a' && hexSymbol <= 'f')
    {
        return 10 + (hexSymbol - 'a');
    }
    else
    {
        throw std::invalid_argument("invalid hex character");
    }

    return -1;
}

std::vector<uint8_t> hexStringToBytes(const char* str, size_t size)
{
    if (size % 2 != 0)
    {
        throw std::invalid_argument("hex string must have even length");
    }

    std::vector<uint8_t> result;
    result.reserve(size / 2);

    for (size_t i = 0; i < size; i += 2)
    {
        uint8_t high = hexToByte(str[i]);
        uint8_t low = hexToByte(str[i + 1]);

        result.push_back((high << 4) | low);
    }

    return result;
}