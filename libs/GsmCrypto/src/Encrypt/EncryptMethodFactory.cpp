#include <GsmCrypto/Encrypt/EncryptMethodFactory.h>

#include <GsmCrypto/Encrypt/EcnryptA51.h>
#include <GsmCrypto/Encrypt/EcnryptA52.h>

#include <stdexcept>

std::unique_ptr<EncryptMethod> EncryptMethodFactory::create(uint8_t algoId)
{
    switch (algoId)
    {
        case 1:
            return std::make_unique<EncryptA51>();
        case 2:
            return std::make_unique<EncryptA52>();
        default:
            throw std::runtime_error("EncryptMethodFactory: unknown algorithm ID");
    }
}
