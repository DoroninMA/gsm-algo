#ifndef _NETWORK_LEVEL3_VOICE_FRAME_H
#define _NETWORK_LEVEL3_VOICE_FRAME_H

#include <Network/GsmMessage.h>

class VoiceMessage : public GsmMessage
{
public:
    VoiceMessage() = default;
    explicit VoiceMessage(const std::vector<uint8_t>& data);

    GsmMessagePD protocolDiscriminator() const final;
    const std::vector<uint8_t>& voiceData() const;

    uint8_t messageType() const override;

    void parse(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> pack() const override;

    void setVoiceData(const std::vector<uint8_t>& data);

private:
    std::vector<uint8_t> _voiceData;
};

#endif
