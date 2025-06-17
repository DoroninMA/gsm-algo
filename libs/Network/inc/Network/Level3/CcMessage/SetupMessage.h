#ifndef _NETWORK_LEVEL3_СС_MESSAGE_SETUP_MESSAGE_H
#define _NETWORK_LEVEL3_СС_MESSAGE_SETUP_MESSAGE_H

#include <Network/Tlv.h>
#include <Network/GsmMessage.h>

#include <string>

#include "CcMessage.h"

class SetupMessage final : public CcMessage {
public:
    SetupMessage();
    SetupMessage(const std::vector<uint8_t>& bearerCap,
                 const std::string& calledParty,
                 const std::string& callingParty = "");

    std::vector<uint8_t> pack() const override;
    uint8_t messageType() const override;

    std::vector<uint8_t> getBearerCapability() const;
    const std::string& calledPartyNumber() const;
    const std::string& callingPartyNumber() const;
    bool isCallingPartyNumberExist() const;

    void parse(const std::vector<uint8_t>& data) override;
    void setBearerCapability(const std::vector<uint8_t>& bc);
    void setCalledPartyNumber(const std::string& num);
    void setCallingPartyNumber(const std::string& num);

private:
    Tlv _bearerCapability;             // IE tag 0x04
    std::string _calledPartyNumber;    // BCD string
    bool _isCallingPartyExist;
    std::string _callingPartyNumber;   // BCD string, IE tag 0x0A
};

#endif
