#ifndef _GSM_CRYPTO_ENCRYPT_ECNRYPT_A_5_1_H
#define _GSM_CRYPTO_ENCRYPT_ECNRYPT_A_5_1_H

#include "EcnryptMethod.h"

class EncryptA51 : public EncryptMethod
{
public:
    EncryptA51() = default;
    ~EncryptA51() override = default;

    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data) override;

private:
    std::vector<uint8_t> crypt(const std::vector<uint8_t>& data);
};

#endif
