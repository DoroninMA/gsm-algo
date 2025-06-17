#ifndef NETWORK_ITRANSPORT_H
#define NETWORK_ITRANSPORT_H

#include <functional>

class ITransport
{
public:
    using SendHandler = std::function<void(const std::error_code&, std::size_t)>;
    using ReceiveHandler = std::function<void(
        const std::error_code&,
        std::size_t,
        const std::vector<uint8_t>&,
        const std::string&, uint16_t)>;

    virtual ~ITransport() = default;

    virtual void send(const std::vector<uint8_t>& data) = 0;
    virtual void asyncSend(const std::vector<uint8_t>& data) = 0;

    virtual std::vector<uint8_t> receive() = 0;
    virtual void asyncReceive() = 0;

    virtual void setOnReceive(ReceiveHandler handler) = 0;
    virtual void setOnSend(SendHandler handler) = 0;
};

#endif
