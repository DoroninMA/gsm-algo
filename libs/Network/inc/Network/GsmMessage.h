#ifndef _NETWORK_GSM_MESSAGE_H
#define _NETWORK_GSM_MESSAGE_H

#include <vector>
#include <cstdint>

class GsmMessage
{
public:
    virtual ~GsmMessage() = default;

    virtual uint8_t getMessageType() const = 0;

    virtual void parse(const std::vector<uint8_t>& data) = 0;
    virtual std::vector<uint8_t> pack() const = 0;
};

#endif
