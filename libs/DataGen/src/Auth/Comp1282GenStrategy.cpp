#include "DataGen/Auth/Comp1282GenStrategy.h"

extern "C"
{
    #include "GsmCrypto/comp128v23.h"
}

Comp1282GenStrategy::Comp1282GenStrategy() : AuthDataGenStrategy()
{
}

std::vector<uint8_t> Comp1282GenStrategy::generateNext()
{
    comp128v2(getKi().data(), getRand().data(), _output.data(), _output.data() + SRES_BYTES);
    return _output;
}
