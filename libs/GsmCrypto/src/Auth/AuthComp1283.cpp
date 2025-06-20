#include "Comp1283.h"
#include <stdexcept>

extern "C" {
#include <GsmCrypto/comp128v23.h>
}

void Comp1283::generateNext(std::vector<uint8_t>& sres, std::vector<uint8_t>& kc)
{
    sres.resize(16);
    kc.resize(16);

    uint8_t out[12];
    comp128v3(ki().data(), rand().data(), out)

    sres.assign(out, out + 4);
    kc.assign(out + 4, out + 12);
}
