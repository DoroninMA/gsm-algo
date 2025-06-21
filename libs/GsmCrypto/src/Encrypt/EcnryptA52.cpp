#include <GsmCrypto/Encrypt/EcnryptA52.h>

extern "C" {
#include <GsmCrypto/a5.h>
}

void EncryptA52::_generateKeyStream(const uint8_t* key, uint32_t frameNumber, uint8_t* decBits, uint8_t* encBits)
{
    osmo_a5_2(key, frameNumber, decBits, encBits);
}
