#ifndef _NETWORK_UDP_SOCKET_H
#define _NETWORK_UDP_SOCKET_H

#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <system_error>

#include "ITransport.h"

class UdpSocket : public ITransport {
public:
    static const size_t DEFAULT_BUFFER_SIZE;

    explicit UdpSocket(
        boost::asio::io_context& ioContext,
        uint16_t localPort = 0,
        size_t bufferSize = DEFAULT_BUFFER_SIZE);

    ~UdpSocket() override = default;

    void setRemote(const std::string& remoteAddress, uint16_t remotePort);

    void send(const std::vector<uint8_t>& data) override;
    void asyncSendRequest(const std::vector<uint8_t>& data) override;
    void asyncSendResponse(const std::vector<uint8_t>& data);

    std::vector<uint8_t> receive() override;
    void asyncReceive() override;

    void setOnReceive(ReceiveHandler handler) override;
    void setOnSend(SendHandler handler) override;

private:
    static std::error_code _toStdError(const boost::system::error_code& boostEc);

    boost::asio::io_context& _ioContext;
    boost::asio::ip::udp::socket _socket;

    boost::asio::ip::udp::endpoint _remoteEndpoint;
    boost::asio::ip::udp::endpoint _lastSenderEndpoint;

    std::vector<uint8_t> _recvBuffer;

    ReceiveHandler _receiveHandler;
    SendHandler _sendHandler;
};

#endif
