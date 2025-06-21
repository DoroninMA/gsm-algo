#ifndef _GSM_CRYPTO_ENCRYPT_ENCRYPT_METHOD_H
#define _GSM_CRYPTO_ENCRYPT_ENCRYPT_METHOD_H

#include <cstdint>
#include <vector>

class EncryptMethod
{
public:
    virtual ~EncryptMethod() = default;

    const std::vector<uint8_t>& kc() const;
    uint32_t frameNumber() const;

    void setKc(const std::vector<uint8_t>& kc);
    void setFrameNumber(uint32_t frameNumber);

    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data);

protected:
    virtual void _generateKeyStream(const uint8_t* key, uint32_t frameNumber, uint8_t* decBits, uint8_t* encBits) = 0;

private:
    enum class Direction
    {
        Uplink,
        Downlink
    };

    std::vector<uint8_t> _kc;
    uint32_t _frameNumber = 0;

    std::vector<uint8_t> _encBitKeystreamBuf;
    std::vector<uint8_t> _decBitKeystreamBuf;

    std::vector<uint8_t> _process(const std::vector<uint8_t>& data, Direction dir);
};

#endif
