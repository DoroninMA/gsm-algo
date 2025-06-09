#include "DataGen/Auth/Comp1283GenStrategy.h"

extern "C"
{
    #include "GsmCrypto/comp128v23.h"
}

Comp1283GenStrategy::Comp1283GenStrategy() : AuthDataGenStrategy()
{
}

std::vector<uint8_t> Comp1283GenStrategy::generateNext()
{
    comp128v3(getKi().data(), getRand().data(), _output.data(), _output.data() + SRES_BYTES);
    return _output;
}
