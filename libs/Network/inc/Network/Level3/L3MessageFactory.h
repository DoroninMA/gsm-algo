#ifndef _NETWORK_LEVEL3_L3_MESSAGE_FACTORY_H
#define _NETWORK_LEVEL3_L3_MESSAGE_FACTORY_H

#include <memory>

#include <Network/GsmMessage.h>

class MessageFactory {
public:
    static std::unique_ptr<GsmMessage> parse(const std::vector<uint8_t>& data);
};

#endif
