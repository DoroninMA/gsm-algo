#ifndef _NETWORK_GSM_DEFS_H
#define _NETWORK_GSM_DEFS_H

#include <cstdint>

enum class GsmMsgTypeL3 : uint8_t
{
    AUTH_REQUEST = 0x2B,
    AUTH_RESPONSE = 0x2C,

    CIPHER_MODE_COMMAND = 0x35,
};

#endif
