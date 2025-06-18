#ifndef _NETWORK_GSM_DEFS_H
#define _NETWORK_GSM_DEFS_H

#include <cstdint>

enum class GsmMsgTypeL3 : uint8_t
{
    SETUP = 0x05,
    LOCATION_UPDATE_REQUEST = 0x08,
    CONNECT_ACK = 0x0F,

    VOICE_FRAME = 0xFF,

    AUTH_REQUEST = 0x2B,
    AUTH_RESPONSE = 0x2C,

    CIPHER_MODE_COMMAND = 0x35,

    RELEASE_MESSAGE = 0x4C,
};

// message protocol discriminator
enum class GsmMessagePD : uint8_t
{
    NONE = 0x00,
    CALL_CONTROL = 0x01,
    MOBILITY_MANAGEMENT = 0x02,
    RADIO_RESOURCE_MANAGEMENT = 0x06
};

#endif
