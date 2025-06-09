#include <algorithm>

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