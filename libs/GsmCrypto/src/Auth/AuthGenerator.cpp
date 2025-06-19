#include <GsmCrypto/Auth/AuthGenerator.h>

const size_t AuthGenerator::SRES_BYTES = 4;
const size_t AuthGenerator::KC_BYTES = 8;

const std::vector<uint8_t>& AuthGenerator::ki() const
{
    return _ki;
}

const std::vector<uint8_t>& AuthGenerator::rand() const
{
    return _rand;
}

void AuthGenerator::setKi(const std::vector<uint8_t>& ki)
{
    _ki = ki;
}

void AuthGenerator::setKi(std::vector<uint8_t>&& ki)
{
    _ki = std::move(ki);
}

void AuthGenerator::setRand(const std::vector<uint8_t>& rand)
{
    _rand = rand;
}

void AuthGenerator::setRand(std::vector<uint8_t>&& rand)
{
    _rand = std::move(rand);
}
