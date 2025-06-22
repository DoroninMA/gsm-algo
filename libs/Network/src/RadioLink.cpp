#include "Network/RadioLink.h"

void RadioLink::setReceiveHandler(ITransport::ReceiveHandler handler)
{
    _transport->setOnReceive(std::move(handler));
}

RadioLink::RadioLink(std::shared_ptr<ITransport> transport): _transport(std::move(transport))
{}

void RadioLink::sendRequest(const std::vector<uint8_t>& data)
{
    _transport->asyncSendRequest(data);
}

void RadioLink::sendResponse(const std::vector<uint8_t>& data)
{
    _transport->asyncSendResponse(data);
}

void RadioLink::receive() const
{
    _transport->asyncReceive();
}

void RadioLink::setSendHandler(ITransport::SendHandler handler)
{
    _transport->setOnSend(std::move(handler));
}
