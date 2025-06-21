#ifndef _NETWORK_LEVEL3_MM_MESSAGE_LOCATION_UPDATE_ACCEPT_H
#define _NETWORK_LEVEL3_MM_MESSAGE_LOCATION_UPDATE_ACCEPT_H

#include "MmMessage.h"

class LocationUpdateAccept final : public MmMessage
{
public:
    LocationUpdateAccept() = default;

    explicit LocationUpdateAccept(const std::vector<uint8_t>& lai);

    void setLai(const std::vector<uint8_t>& lai);

    const std::vector<uint8_t>& lai() const;

    uint8_t messageType() const override;

    std::vector<uint8_t> pack() const override;

    void parse(const std::vector<uint8_t>& data) override;

private:
    std::vector<uint8_t> _lai;
};

#endif
