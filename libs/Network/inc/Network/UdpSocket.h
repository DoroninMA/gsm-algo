#ifndef _NETWORK_UDP_SOCKET
#define _NETWORK_UDP_SOCKET

#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include <functional>

class UdpSocket
{
public:
    static const std::size_t DEFAULT_BUFFER_SIZE;

    using SendHandler = std::function<void(const boost::system::error_code&, std::size_t)>;
    using ReceiveHandler = std::function<void(
        const boost::system::error_code&,
        std::size_t,
        const std::vector<uint8_t>&,
        const std::string &, uint16_t)>;

    UdpSocket(boost::asio::io_context& ioContext, uint16_t localPort = 0, std::size_t bufferSize = DEFAULT_BUFFER_SIZE);
    ~UdpSocket();

    std::string getLastSenderAddress() const;
    uint16_t getLastSenderPort() const;

    void setRemote(const std::string& remoteAddress, uint16_t remotePort);

    void send(const std::vector<uint8_t>& data);
    void asyncSend(const std::vector<uint8_t>& data);

    std::vector<uint8_t> receive();
    void asyncReceive();

    void setOnReceive(ReceiveHandler handler);
    void setOnSend(SendHandler handler);

private:
    boost::asio::io_context& _ioContext;
    boost::asio::ip::udp::socket _socket;

    boost::asio::ip::udp::endpoint _remoteEndpoint;
    boost::asio::ip::udp::endpoint _lastSenderEndpoint;

    std::vector<uint8_t> _recvBuffer;

    ReceiveHandler _receiveHandler;
    SendHandler _sendHandler;
};


#endif
