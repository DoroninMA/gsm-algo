#include "Comp1281.h"
#include <stdexcept>

extern "C" {
#include <GsmCrypto/comp128.h>
}

void Comp1281::generateNext(std::vector<uint8_t>& sres, std::vector<uint8_t>& kc)
{
    sres.resize(16);
    kc.resize(16);

    uint8_t out[12];
    comp128_1(ki().data(), rand().data(), out);

    sres.assign(out, out + 4);
    kc.assign(out + 4, out + 12);
}
