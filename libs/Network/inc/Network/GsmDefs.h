#ifndef _NETWORK_GSM_DEFS_H
#define _NETWORK_GSM_DEFS_H

#include <cstdint>

enum class GsmMsgTypeCC : uint8_t
{
    SETUP = 0x05,
    CONNECT_ACK = 0x0F,
    RELEASE = 0x4C,
    RELEASE_COMPLETE = 0x2A,
};

enum class GsmMsgTypeMM : uint8_t
{
    LOCATION_UPDATE_REQUEST = 0x08,
    LOCATION_UPDATE_ACCEPT = 0x10,
    LOCATION_UPDATE_REJECT = 0x04,
    AUTH_REQUEST = 0x2B,
    AUTH_RESPONSE = 0x2C,
    CIPHER_MODE_COMMAND = 0x35,
    CIPHER_MODE_COMPLETE = 0x2A,
};


// message protocol discriminator
enum class GsmMessagePD : uint8_t
{
    NONE = 0x00,
    CALL_CONTROL = 0x01,
    MOBILITY_MANAGEMENT = 0x02,
    RADIO_RESOURCE_MANAGEMENT = 0x06,
    TRAFIC_CHANNEL = 0xFF,
};

enum class GsmLurCause : uint8_t
{
    UNDEFINED = 0,
    AUTH_FAIL = 0x08,
    LA_NOT_ALLOWED = 0x11,
};

#endif
