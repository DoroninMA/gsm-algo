#ifndef _NETWORK_RADIO_LINK_H
#define _NETWORK_RADIO_LINK_H

#include "ITransport.h"

#include <memory>
#include <vector>

class RadioLink {
public:
    explicit RadioLink(std::shared_ptr<ITransport> transport);

    void send(const std::vector<uint8_t>& data);
    void receive() const;

    void setReceiveHandler(ITransport::ReceiveHandler handler);
    void setSendHandler(ITransport::SendHandler handler);

private:
    std::shared_ptr<ITransport> _transport;
};

#endif
