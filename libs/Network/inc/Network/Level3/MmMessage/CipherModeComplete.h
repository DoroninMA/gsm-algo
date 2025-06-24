#ifndef _NETWORK_LEVEL3_MM_MESSAGE_CIPHER_MODE_COMPLETE_H
#define _NETWORK_LEVEL3_MM_MESSAGE_CIPHER_MODE_COMPLETE_H

#include "Network/Level3/MmMessage/MmMessage.h"
#include "Network/Tlv.h"

class CipherModeComplete final : public MmMessage
{
public:
    CipherModeComplete();

    uint8_t messageType() const override;
    uint8_t cryptoAlgoId() const;
    bool isCryptoAlgoIdExist() const;

    std::vector<uint8_t> pack() const override;
    void parse(const std::vector<uint8_t>& data) override;

    void setCryptoAlgoId(uint8_t cryptoAlgoId);

private:
    uint8_t _cryptoAlgoId;
};

#endif
