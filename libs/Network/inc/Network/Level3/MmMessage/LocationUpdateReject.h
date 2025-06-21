#ifndef _NETWORK_LEVEL3_MM_MESSAGE_LOCATION_UPDATE_REJECT_H
#define _NETWORK_LEVEL3_MM_MESSAGE_LOCATION_UPDATE_REJECT_H

#include "MmMessage.h"
#include "Network/Tlv.h"


class LocationUpdateReject final : public MmMessage
{
public:
    LocationUpdateReject() = default;
    explicit LocationUpdateReject(uint8_t cause);

    uint8_t cause() const;
    uint8_t messageType() const override;

    std::vector<uint8_t> pack() const override;
    void parse(const std::vector<uint8_t>& data) override;

    void setCause(uint8_t cause);

private:
    Tlv _cause;
};

#endif
