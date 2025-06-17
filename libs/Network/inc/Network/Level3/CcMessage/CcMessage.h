#ifndef _NETWORK_LEVEL3_СС_MESSAGE_H
#define _NETWORK_LEVEL3_СС_MESSAGE_H

#include "Network/GsmMessage.h"

class CcMessage : public GsmMessage
{
public:
    void parse(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> pack() const override;
    virtual uint8_t messageType() const = 0;

    uint8_t transactionId() const;

protected:
    uint8_t _ti;

    CcMessage();
};

#endif
