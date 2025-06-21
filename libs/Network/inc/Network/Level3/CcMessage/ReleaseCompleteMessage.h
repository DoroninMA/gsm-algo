#ifndef _NETWORK_LEVEL3_СС_MESSAGE_RELEASE_COMPLETE_MESSAGE_H
#define _NETWORK_LEVEL3_СС_MESSAGE_RELEASE_COMPLETE_MESSAGE_H

#include "CcMessage.h"

class ReleaseCompleteMessage final : public CcMessage
{
public:
    uint8_t messageType() const override;

    void parse(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> pack() const override;
};

#endif
