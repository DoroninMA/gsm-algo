#ifndef _GSM_CRYPTO_ENCRYPT_ENCRYPT_METHOD_FACTORY_H
#define _GSM_CRYPTO_ENCRYPT_ENCRYPT_METHOD_FACTORY_H

#include "EcnryptMethod.h"

#include <memory>

class EncryptMethodFactory
{
public:
    static std::unique_ptr<EncryptMethod> create(uint8_t algoId);
};

#endif
