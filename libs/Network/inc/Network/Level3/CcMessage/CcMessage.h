#ifndef _NETWORK_LEVEL3_СС_MESSAGE_H
#define _NETWORK_LEVEL3_СС_MESSAGE_H

#include "Network/GsmMessage.h"

class CcMessage : public GsmMessage
{
public:
    void parse(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> pack() const override;

    uint8_t transactionId() const;
    void setTransactionId(uint8_t transactionId);

protected:
    uint8_t _tid;

    CcMessage();
};

#endif
