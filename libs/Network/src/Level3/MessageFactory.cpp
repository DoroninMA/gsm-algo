#include <Network/Level3/L3MessageFactory.h>

#include <stdexcept>

#include "Network/Level3/CcMessage/SetupMessage.h"

#include "Network/Level3/MmMessage/AuthRequestMessage.h"
#include "Network/Level3/MmMessage/AuthResponse.h"
#include "Network/Level3/MmMessage/CipherModeCommand.h"
#include "Network/Level3/MmMessage/LocationUpdateRequest.h"


std::unique_ptr<GsmMessage> MessageFactory::parse(const std::vector<uint8_t>& data)
{
    if (data.size() < 2)
    {
        throw std::runtime_error("MessageFactory: data too short");
    }

    uint8_t pd = data[0];
    uint8_t messageType = data[1];

    std::unique_ptr<GsmMessage> message;

    switch (pd)
    {
        case static_cast<uint8_t>(GsmMessagePD::MOBILITY_MANAGEMENT):
            switch (messageType)
            {
            case static_cast<uint8_t>(GsmMsgTypeL3::AUTH_REQUEST):
                message = std::make_unique<AuthRequestMessage>();
                break;
            case static_cast<uint8_t>(GsmMsgTypeL3::AUTH_RESPONSE):
                    message = std::make_unique<AuthResponseMessage>();
                    break;
            case static_cast<uint8_t>(GsmMsgTypeL3::CIPHER_MODE_COMMAND):
                    message = std::make_unique<CipherModeCommand>();
                    break;
            case static_cast<uint8_t>(GsmMsgTypeL3::LOCATION_UPDATE_REQUEST):
                    message = std::make_unique<LocationUpdateRequest>();
                    break;
            default:
                throw std::runtime_error("MessageFactory: Unknown MM message type");
            }
            break;

        case static_cast<uint8_t>(GsmMessagePD::CALL_CONTROL):
            switch (messageType)
            {
            case static_cast<uint8_t>(GsmMsgTypeL3::SETUP):
                message = std::make_unique<SetupMessage>();
                break;
            default:
                throw std::runtime_error("MessageFactory: Unknown CC message type");
            }
            break;

        default:
            throw std::runtime_error("MessageFactory: Unknown protocol discriminator");
    }

    message->parse(data);
    return message;
}
