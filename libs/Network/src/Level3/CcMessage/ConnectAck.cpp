#include <Network/Level3/CcMessage/ConnectAck.h>

#include <iostream>
#include <stdexcept>

uint8_t ConnectAck::messageType() const
{
    return static_cast<uint8_t>(GsmMsgTypeCC::CONNECT_ACK);
}

std::vector<uint8_t> ConnectAck::pack() const
{
    std::cout << "ConnectAck::pack\n";
    std::vector<uint8_t> out = CcMessage::pack();
    out.push_back(messageType());
    return out;
}

void ConnectAck::parse(const std::vector<uint8_t>& data)
{
    std::cout << "ConnectAck::parse\n";

    CcMessage::parse(data);
    if (data.size() < 2)
    {
        throw std::runtime_error("ConnectAcknowledgeMessage: too short");
    }

    if (data[1] != messageType())
    {
        throw std::runtime_error("ConnectAcknowledgeMessage: wrong message type");
    }
}
