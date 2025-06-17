#include "Network/RadioLink.h"

void RadioLink::setReceiveHandler(ITransport::ReceiveHandler handler)
{
    _transport->setOnReceive(std::move(handler));
}

RadioLink::RadioLink(std::shared_ptr<ITransport> transport): _transport(std::move(transport))
{}

void RadioLink::send(const std::vector<uint8_t>& data)
{
    _transport->asyncSend(data);
}

void RadioLink::receive() const
{
    _transport->asyncReceive();
}

void RadioLink::setSendHandler(ITransport::SendHandler handler)
{
    _transport->setOnSend(std::move(handler));
}
