#include "DataGen/Auth/Comp1281GenStrategy.h"

extern "C"
{
    #include "GsmCrypto/comp128.h"
}

Comp1281GenStrategy::Comp1281GenStrategy() : AuthDataGenStrategy()
{
}

std::vector<uint8_t> Comp1281GenStrategy::generateNext()
{
    //comp128(getKi().data(), getRand().data(), _sres.data(), _kc.data());
    //return _sres + _kc;

    comp128(getKi().data(), getRand().data(), _output.data(), _output.data() + SRES_BYTES);
    return _output;
}
