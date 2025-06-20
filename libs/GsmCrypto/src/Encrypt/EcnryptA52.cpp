#include "EcnryptA52.h"

extern "C" {
#include <GsmCrypto/a5.h>
}

std::vector<uint8_t> EncryptA52::encrypt(const std::vector<uint8_t>& data)
{
    return crypt(data);
}

std::vector<uint8_t> EncryptA52::decrypt(const std::vector<uint8_t>& data)
{
    return crypt(data);
}

std::vector<uint8_t> EncryptA52::crypt(const std::vector<uint8_t>& data)
{
    if (kc().size() < 8)
    {
        throw std::runtime_error("A5_2: key Kc must be at least 8 bytes");
    }

    size_t bitsLength = data.size() * 8;
    std::vector<uint8_t> keystream(data.size(), 0);

    osmo_a5_2(kc().data(), frameNumber(), keystream.data(), bitsLength);

    std::vector<uint8_t> output(data.size());
    for (size_t i = 0; i < data.size(); i++)
    {
        output[i] = data[i] ^ keystream[i];
    }

    setFrameNumber(frameNumber() + 1);

    return output;
}
