#include <GsmCrypto/Auth/AuthComp1283.h>

extern "C" {
#include <GsmCrypto/comp128v23.h>
}

void Comp1283::generateNext(std::vector<uint8_t>& sres, std::vector<uint8_t>& kc)
{
    sres.resize(4, 0x00);
    kc.resize(8, 0x00);

    comp128v3(ki().data(), rand().data(), sres.data(), kc.data());
}
