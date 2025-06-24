#include <iostream>
#include <stdexcept>
#include <Network/Level2/VoiceMessage.h>

VoiceMessage::VoiceMessage(const std::vector<uint8_t>& data) : VoiceMessage()
{
    _voiceData = data;
}

GsmMessagePD VoiceMessage::protocolDiscriminator() const
{
    return GsmMessagePD::TRAFIC_CHANNEL;
}

const std::vector<uint8_t>& VoiceMessage::voiceData() const
{
    return _voiceData;
}

uint8_t VoiceMessage::messageType() const
{
    return 0xFF;
}

void VoiceMessage::parse(const std::vector<uint8_t>& data)
{
    std::cout << "VoiceFrame::parse" << std::endl;

    if (data.empty())
    {
        throw std::invalid_argument("VoiceFrame error: empty voiceData");
    }

    if ((data[0] >> 4) != static_cast<uint8_t>(protocolDiscriminator()))
    {
        throw std::invalid_argument("VoiceFrame error: invalid protocolDiscriminator");
    }

    _voiceData.resize(data.size() - 1);
    std::copy(data.cbegin() + 1, data.cend(), _voiceData.begin());
}

std::vector<uint8_t> VoiceMessage::pack() const
{
    std::cout << "VoiceFrame::pack" << std::endl;

    std::vector<uint8_t> out(_voiceData.size() + 1);
    out[0] = (static_cast<uint8_t>(protocolDiscriminator()) << 4);

    std::copy(_voiceData.cbegin(), _voiceData.cend(), out.begin() + 1);
    return out;
}

void VoiceMessage::setVoiceData(const std::vector<uint8_t>& data)
{
    _voiceData = data;
}
