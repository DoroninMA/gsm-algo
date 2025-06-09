#include "DataGen/Auth/AuthDataGenStrategy.h"

const size_t AuthDataGenStrategy::SRES_BYTES = 4;
const size_t AuthDataGenStrategy::KC_BYTES = 8;

AuthDataGenStrategy::AuthDataGenStrategy()
{
    _output.resize(SRES_BYTES + KC_BYTES);
}

const std::vector<uint8_t>& AuthDataGenStrategy::getKi() const
{
    return _ki;
}

const std::vector<uint8_t>& AuthDataGenStrategy::getRand() const
{
    return _rand;
}

void AuthDataGenStrategy::setKi(const std::vector<uint8_t>& ki)
{
    _ki = ki;
}

void AuthDataGenStrategy::setRand(const std::vector<uint8_t>& rand)
{
    _rand = rand;
}
