#include <Network/Level3/MmMessage/CipherModeComplete.h>

#include <iostream>
#include <stdexcept>

static constexpr uint8_t CRYPTO_ALGO_ID_NOT_SET = 0xFF;

CipherModeComplete::CipherModeComplete() : _cryptoAlgoId(CRYPTO_ALGO_ID_NOT_SET)
{
}

uint8_t CipherModeComplete::messageType() const
{
    return static_cast<uint8_t>(GsmMsgTypeMM::CIPHER_MODE_COMPLETE);
}

uint8_t CipherModeComplete::cryptoAlgoId() const
{
    return _cryptoAlgoId;
}

bool CipherModeComplete::isCryptoAlgoIdExist() const
{
    return _cryptoAlgoId != CRYPTO_ALGO_ID_NOT_SET;
}

std::vector<uint8_t> CipherModeComplete::pack() const
{
    std::cout << "CipherModeComplete::pack\n";
    std::vector<uint8_t> out = MmMessage::pack();

    if (isCryptoAlgoIdExist())
    {
        out.push_back(cryptoAlgoId());
    }

    return out;
}

void CipherModeComplete::parse(const std::vector<uint8_t>& data)
{
    std::cout << "CipherModeComplete::parse\n";

    MmMessage::parse(data);
    size_t offset = 2;

    if (offset < data.size())
    {
        _cryptoAlgoId = data[offset];
    }
}

void CipherModeComplete::setCryptoAlgoId(uint8_t cryptoAlgoId)
{
    _cryptoAlgoId = cryptoAlgoId;
}
