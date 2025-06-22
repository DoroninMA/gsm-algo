#include "Network/Level3/CcMessage/ReleaseCompleteMessage.h"

#include <iostream>
#include <stdexcept>

uint8_t ReleaseCompleteMessage::messageType() const
{
    return static_cast<uint8_t>(GsmMsgTypeCC::RELEASE_COMPLETE);
}

void ReleaseCompleteMessage::parse(const std::vector<uint8_t>& data)
{
    std::cout << "ReleaseCompleteMessage::pack\n";

    CcMessage::parse(data);

    if (data.size() < 2)
    {
        throw std::runtime_error("ReleaseCompleteMessage: too short");
    }

    uint8_t msgType = data[2];
    if (msgType != messageType())
    {
        throw std::runtime_error("ReleaseCompleteMessage: invalid message type");
    }
}

std::vector<uint8_t> ReleaseCompleteMessage::pack() const
{
    std::cout << "ReleaseCompleteMessage::parse\n";
    std::vector<uint8_t> out = CcMessage::pack();
    out.push_back(messageType());
    return out;
}
