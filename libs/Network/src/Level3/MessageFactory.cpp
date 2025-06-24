#include <Network/Level3/L3MessageFactory.h>

#include <stdexcept>

#include <Network/Level3/MmMessage/LocationUpdateAccept.h>
#include <Network/Level3/MmMessage/LocationUpdateReject.h>

#include "Network/Level3/CcMessage/SetupMessage.h"

#include "Network/Level3/MmMessage/AuthRequestMessage.h"
#include "Network/Level3/MmMessage/AuthResponse.h"
#include "Network/Level3/MmMessage/CipherModeCommand.h"
#include "Network/Level3/MmMessage/CipherModeComplete.h"
#include "Network/Level3/MmMessage/LocationUpdateRequest.h"


std::unique_ptr<GsmMessage> MessageFactory::parse(const std::vector<uint8_t>& data)
{
    if (data.size() < 2)
    {
        throw std::runtime_error("MessageFactory: data too short");
    }

    uint8_t pd = data[0] >> 4;
    uint8_t messageType = data[1];

    std::unique_ptr<GsmMessage> message;

    switch (pd)
    {
        case static_cast<uint8_t>(GsmMessagePD::MOBILITY_MANAGEMENT):
            switch (messageType)
            {
            case static_cast<uint8_t>(GsmMsgTypeMM::AUTH_REQUEST):
                message = std::make_unique<AuthRequestMessage>();
                break;
            case static_cast<uint8_t>(GsmMsgTypeMM::AUTH_RESPONSE):
                message = std::make_unique<AuthResponseMessage>();
                break;
            case static_cast<uint8_t>(GsmMsgTypeMM::CIPHER_MODE_COMMAND):
                message = std::make_unique<CipherModeCommand>();
                break;
            case static_cast<uint8_t>(GsmMsgTypeMM::CIPHER_MODE_COMPLETE):
                message = std::make_unique<CipherModeComplete>();
                break;
            case static_cast<uint8_t>(GsmMsgTypeMM::LOCATION_UPDATE_REQUEST):
                message = std::make_unique<LocationUpdateRequest>();
                break;
            case static_cast<uint8_t>(GsmMsgTypeMM::LOCATION_UPDATE_ACCEPT):
                message = std::make_unique<LocationUpdateAccept>();
                break;
            case static_cast<uint8_t>(GsmMsgTypeMM::LOCATION_UPDATE_REJECT):
                message = std::make_unique<LocationUpdateReject>();
                break;
            default:
                throw std::runtime_error("MessageFactory: Unknown MM message type");
            }
            break;

        case static_cast<uint8_t>(GsmMessagePD::CALL_CONTROL):
            switch (messageType)
            {
            case static_cast<uint8_t>(GsmMsgTypeCC::SETUP):
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
