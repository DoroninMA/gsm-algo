#include <GsmCrypto/Auth/AuthComp1282.h>

extern "C" {
#include <GsmCrypto/comp128v23.h>
}

void Comp1282::generateNext(std::vector<uint8_t>& sres, std::vector<uint8_t>& kc)
{
    sres.resize(4, 0x00);
    kc.resize(12, 0x00);

    comp128v2(ki().data(), rand().data(), sres.data(), kc.data());
}
