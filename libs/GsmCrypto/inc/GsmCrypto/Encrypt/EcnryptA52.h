#ifndef _GSM_CRYPTO_ENCRYPT_ENCRYPT_A_5_2_H
#define _GSM_CRYPTO_ENCRYPT_ENCRYPT_A_5_2_H

#include "EcnryptMethod.h"

class EncryptA52 final : public EncryptMethod
{
    void _generateKeyStream(const uint8_t* key, uint32_t frameNumber, uint8_t* decBits, uint8_t* encBits) override;
};

#endif
