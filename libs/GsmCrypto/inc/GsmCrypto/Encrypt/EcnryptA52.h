#ifndef _GSM_CRYPTO_ENCRYPT_ECNRYPT_A_5_2_H
#define _GSM_CRYPTO_ENCRYPT_ECNRYPT_A_5_2_H

#include "EcnryptMethod.h"

class EncryptA52 : public EncryptMethod
{
public:
    EncryptA52() = default;
    ~EncryptA52() override = default;

    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data) override;

private:
    std::vector<uint8_t> crypt(const std::vector<uint8_t>& data);
};

#endif
