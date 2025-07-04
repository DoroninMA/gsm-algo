#ifndef NETWORK_ITRANSPORT_H
#define NETWORK_ITRANSPORT_H

#include <cstdint>
#include <functional>
#include <system_error>

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
    virtual void asyncSendRequest(const std::vector<uint8_t>& data) = 0;
    virtual void asyncSendResponse(const std::vector<uint8_t>& data) = 0;

    virtual std::vector<uint8_t> receive() = 0;
    virtual void asyncReceive() = 0;

    virtual void setOnReceive(ReceiveHandler handler) = 0;
    virtual void setOnSend(SendHandler handler) = 0;
};

#endif
