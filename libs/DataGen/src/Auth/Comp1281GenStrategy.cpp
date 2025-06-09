#include "DataGen/Auth/Comp1281GenStrategy.h"

extern "C"
{
    #include "GsmCrypto/comp128.h"
}

static const size_t SRES_BYTES = 4;
static const size_t KC_BYTES   = 8;

Comp1281GenStrategy::Comp1281GenStrategy()
{
    //_kc.resize(KC_BYTES);
    //_sres.resize(SRES_BYTES);
    _output.resize(SRES_BYTES + KC_BYTES);
}

std::vector<uint8_t> Comp1281GenStrategy::generateNext()
{
    //comp128(getKi().data(), getRand().data(), _sres.data(), _kc.data());
    //return _sres + _kc;

    comp128(getKi().data(), getRand().data(), _output.data(), _output.data() + SRES_BYTES);
    return _output;
}
