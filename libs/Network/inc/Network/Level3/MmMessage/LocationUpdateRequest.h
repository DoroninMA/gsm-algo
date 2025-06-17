#ifndef _NETWORK_LEVEL3_MM_MESSAGE_LOCATION_UPDATE_REQUEST_H
#define _NETWORK_LEVEL3_MM_MESSAGE_LOCATION_UPDATE_REQUEST_H

#include <Network/MobileIdentity.h>
#include "MmMessage.h"

/*
+-------------------------------+-----------------+-----------------------------------------+
| Field                         | Size            | Description                             |
+-------------------------------+-----------------+-----------------------------------------+
| Protocol Discriminator        | 1 byte           | 0x05 (Mobility Management)             |
| Skip Indicator + Message Type | 1 byte           | message type  0x08                     |
| Location Updating Type        | 1 byte           | 0x01 (normal update)                   |
| LAI (Location Area Identity)  | 5 bytes          | MCC + MNC + LAC                        |
| Mobile Identity               | IMSI/TMIS size   | IMSI или TMSI, BCD-coding              |
| Cipher Key Sequence Number    | 1 byte (optional)|                                        |
+-------------------------------+-----------------+-----------------------------------------+
*/


class LocationUpdatingRequest final : public MmMessage
{
public:
    LocationUpdatingRequest();
    LocationUpdatingRequest(
        uint8_t locationUpdateType,
        const std::vector<uint8_t>& lai,
        const MobileIdentity& mobileIdentity,
        uint8_t cipherKeySeqNum = 0
    );

    uint8_t messageType() const override;
    uint8_t cipherKeySeqNum() const;
    bool isCipherKeySequenceNumberExist() const;
    uint8_t locationUpdateType() const;
    MobileIdentity mobileIdentity() const;
    std::vector<uint8_t> lai() const;

    void setLocationUpdateType(uint8_t type);
    void setLai(const std::vector<uint8_t>& lai);
    void setMobileIdentity(const MobileIdentity& mi);
    void setCipherKeySeqNum(uint8_t seq);

    std::vector<uint8_t> pack() const override;
    void parse(const std::vector<uint8_t>& data) override;

private:
    uint8_t _locationUpdateType;
    std::vector<uint8_t> _lai;
    MobileIdentity _mobileIdentity;
    bool _isCipherKeySeqNumExist;
    uint8_t _cipherKeySeqNum;
};

#endif
