#include <GsmCrypto/Encrypt/EcnryptMethod.h>

#include <array>

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

std::vector<uint8_t> EncryptMethod::encrypt(const std::vector<uint8_t>& data)
{
    return _process(data, Direction::Uplink);
}

std::vector<uint8_t> EncryptMethod::decrypt(const std::vector<uint8_t>& data)
{
    return _process(data, Direction::Downlink);
}

std::vector<uint8_t> EncryptMethod::_process(const std::vector<uint8_t>& data, Direction dir)
{
    std::vector<uint8_t>& bitBuf = (dir == Direction::Uplink) ? _encBitKeystreamBuf : _decBitKeystreamBuf;
    std::vector<uint8_t> output;
    size_t totalBits = data.size() * 8;

    // generate more keystream if needed
    while (bitBuf.size() < totalBits)
    {
        std::array<uint8_t, 114> decrypt, encrypt;
        _generateKeyStream(_kc.data(), _frameNumber++, decrypt.data(), encrypt.data());

        if (dir == Direction::Uplink)
        {
            bitBuf.insert(bitBuf.end(), encrypt.begin(), encrypt.end());
        }
        else
        {
            bitBuf.insert(bitBuf.end(), decrypt.begin(), decrypt.end());
        }
    }

    // apply XOR bit by bit
    for (size_t i = 0; i < data.size(); ++i)
    {
        uint8_t byte = 0;
        for (int b = 0; b < 8; ++b)
        {
            uint8_t dataBit = (data[i] >> (7 - b)) & 1;
            uint8_t cipherBit = bitBuf[i * 8 + b];
            byte |= (dataBit ^ cipherBit) << (7 - b);
        }
        output.push_back(byte);
    }

    // remove used bits
    bitBuf.erase(bitBuf.begin(), bitBuf.begin() + totalBits);

    return output;
}
