#include <GsmCrypto/Encrypt/EcnryptMethod.h>

const std::vector<uint8_t>& EncryptMethod::kc() const
{
    return _kc;
}

void EncryptMethod::setKc(const std::vector<uint8_t>& kc)
{
    _kc = kc;
}

uint32_t EncryptMethod::frameNumber() const
{
    return _frameNumber;
}

void EncryptMethod::setFrameNumber(uint32_t frameNumber)
{
    _frameNumber = frameNumber;
}
