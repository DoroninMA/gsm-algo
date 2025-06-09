#include <vector>
#include <random>
#include <chrono>

#include "DataGen/RandomString.h"


RandomU8IStringGenerator::RandomU8IStringGenerator(size_t length)
{
    _length = length;
    _rng = std::mt19937(_rd());
}

std::vector<uint8_t> RandomU8IStringGenerator::generateNext()
{
    std::vector<uint8_t> str(_length);
    for (size_t j = 0; j < _length; ++j)
    {
        str[j] = static_cast<uint8_t>(_dist(_rng));
    }

    return str;
}
