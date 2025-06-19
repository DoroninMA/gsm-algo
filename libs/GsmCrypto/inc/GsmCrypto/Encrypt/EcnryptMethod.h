#ifndef _GSM_CRYPTO_ENCRYPT_ECNRYPT_METHOD_H
#define _GSM_CRYPTO_ENCRYPT_ECNRYPT_METHOD_H

#include <cstdint>
#include <vector>

class EncryptMethod
{
public:
    EncryptMethod() = default;
    virtual ~EncryptMethod() = default;

    virtual std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data) = 0;
    virtual std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data) = 0;

    const std::vector<uint8_t>& kc() const;
    uint32_t frameNumber() const;

    void setKc(const std::vector<uint8_t>& kc);
    void setFrameNumber(uint32_t frameNumber);

private:
    std::vector<uint8_t> _kc;
    uint32_t _frameNumber = 0;
};

#endif
