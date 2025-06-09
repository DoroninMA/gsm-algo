#include "DataGen/Auth/AuthDataGenStrategy.h"

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
