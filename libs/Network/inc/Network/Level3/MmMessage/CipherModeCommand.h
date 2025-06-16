#ifndef _NETWORK_LEVEL3_CIPHER_MODE_COMMAND_H
#define _NETWORK_LEVEL3_CIPHER_MODE_COMMAND_H

#include <Network/Tlv.h>
#include <Network/GsmMessage.h>

class CipherModeCommand final : public GsmMessage {
public:
    CipherModeCommand();

    uint8_t getMessageType() const override;
    uint8_t getCipherAlgorithm() const;
    uint8_t getKeySequence() const;
    bool isMobileIdentityExist() const;
    std::vector<uint8_t> getMobileIdentity() const;

    void setCipherAlgorithm(uint8_t algo);
    void setKeySequence(uint8_t seq);
    void setMobileIdentity(const std::vector<uint8_t>& mi);

    void parse(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> pack() const override;

private:
    uint8_t _cipherAlgorithm;
    uint8_t _keySequence;
    Tlv _mobileIdentity;
    bool _isMobileIdentityExist;
};

#endif
