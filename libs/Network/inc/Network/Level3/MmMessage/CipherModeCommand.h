#ifndef _NETWORK_LEVEL3_MM_MESSAGE_CIPHER_MODE_COMMAND_H
#define _NETWORK_LEVEL3_MM_MESSAGE_CIPHER_MODE_COMMAND_H

#include <Network/Tlv.h>

#include "MmMessage.h"

class CipherModeCommand final : public MmMessage
{
public:
    CipherModeCommand();

    uint8_t messageType() const override;
    uint8_t cipherAlgorithm() const;
    uint8_t keySequence() const;
    bool isMobileIdentityExist() const;
    std::vector<uint8_t> mobileIdentity() const;
    std::vector<uint8_t> pack() const override;

    void setCipherAlgorithm(uint8_t algo);
    void setKeySequence(uint8_t seq);
    void setMobileIdentity(const std::vector<uint8_t>& mi);
    void parse(const std::vector<uint8_t>& data) override;

private:
    uint8_t _cipherAlgorithm;
    uint8_t _keySequence;
    Tlv _mobileIdentity;
    bool _isMobileIdentityExist;
};

#endif
