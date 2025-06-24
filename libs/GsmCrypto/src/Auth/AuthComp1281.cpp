#include <GsmCrypto/Auth/AuthComp1281.h>

extern "C" {
#include <GsmCrypto/comp128.h>
}

void Comp1281::generateNext(std::vector<uint8_t>& sres, std::vector<uint8_t>& kc)
{
    sres.resize(4, 0x00);
    kc.resize(8, 0x00);

    comp128v1(ki().data(), rand().data(), sres.data(), kc.data());
}
