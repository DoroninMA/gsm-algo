#ifndef _NETWORK_LEVEL3_MM_MESSAGE_CIPHER_MODE_COMPLETE_H
#define _NETWORK_LEVEL3_MM_MESSAGE_CIPHER_MODE_COMPLETE_H

#include "Network/Level3/MmMessage/MmMessage.h"
#include "Network/Tlv.h"

class CipherModeComplete final : public MmMessage
{
public:
    static constexpr uint8_t TLV_MOBILE_IDENTITY_TAG = 0x17;

    CipherModeComplete();

    uint8_t messageType() const override;
    bool isMobileIdentityExist() const;
    std::vector<uint8_t> mobileIdentity() const;

    void parse(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> pack() const override;

    void setMobileIdentity(const std::vector<uint8_t>& mi);

private:
    bool _isMobileIdentityExist;
    Tlv _mobileIdentity;
};

#endif
