#include <Network/UdpSocket.h>

#include <iostream>

static std::string _bytesToHexString(const uint8_t* str, size_t bytes)
{
    static const char hexChars[] = "0123456789ABCDEF";
    std::string result;
    result.reserve(bytes * 2);

    for (size_t i = 0; i < bytes; ++i)
    {
        uint8_t byte = str[i];
        result.push_back(hexChars[byte >> 4]);
        result.push_back(hexChars[byte & 0x0F]);
    }

    return result;
}

const size_t UdpSocket::DEFAULT_BUFFER_SIZE = 2048;

UdpSocket::UdpSocket(boost::asio::io_context& ioContext, uint16_t localPort, size_t bufferSize)
    : _ioContext(ioContext),
      _socket(ioContext,
          boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), localPort)),
      _recvBuffer(bufferSize)
{}

void UdpSocket::setRemote(const std::string& remoteAddress, uint16_t remotePort)
{
    _remoteEndpoint = boost::asio::ip::udp::endpoint(
        boost::asio::ip::address::from_string(remoteAddress),
        remotePort
    );
}

void UdpSocket::send(const std::vector<uint8_t>& data)
{
    std::cout << "Send data: " << _bytesToHexString(data.data(), data.size()) << "\n";
    _socket.send_to(boost::asio::buffer(data), _remoteEndpoint);
}

void UdpSocket::asyncSendRequest(const std::vector<uint8_t>& data)
{
    std::cout << "asyncSendRequest: " << _bytesToHexString(data.data(), data.size()) << "\n";

    _socket.async_send_to(
        boost::asio::buffer(data), _remoteEndpoint,
        [this](const boost::system::error_code& ec, size_t bytes)
        {
            if (_sendHandler)
            {
                _sendHandler(_toStdError(ec), bytes);
            }
        }
    );
}

void UdpSocket::asyncSendResponse(const std::vector<uint8_t>& data)
{
    std::cout << "asyncSendResponse: " << _bytesToHexString(data.data(), data.size()) << "\n";

    _socket.async_send_to(
        boost::asio::buffer(data), _lastSenderEndpoint,
        [this](const boost::system::error_code& ec, size_t bytes)
        {
            if (_sendHandler)
            {
                _sendHandler(_toStdError(ec), bytes);
            }
        }
    );
}

std::vector<uint8_t> UdpSocket::receive()
{
    boost::asio::ip::udp::endpoint senderEndpoint;
    std::vector<uint8_t> buffer(DEFAULT_BUFFER_SIZE);
    size_t bytes = _socket.receive_from(boost::asio::buffer(buffer), senderEndpoint);

    _lastSenderEndpoint = senderEndpoint;
    buffer.resize(bytes);
    return buffer;
}

void UdpSocket::asyncReceive()
{
    _socket.async_receive_from(
        boost::asio::buffer(_recvBuffer), _lastSenderEndpoint,
        [this](const boost::system::error_code& ec, size_t bytes)
        {
            std::cout << "asyncReceive: " << _bytesToHexString(_recvBuffer.data(), bytes) << "\n";

            if (_receiveHandler)
            {
                std::vector<uint8_t> data(_recvBuffer.begin(), _recvBuffer.begin() + bytes);
                _receiveHandler(_toStdError(ec), bytes, data,
                                _lastSenderEndpoint.address().to_string(),
                                _lastSenderEndpoint.port());
            }

            if (!ec)
            {
                this->asyncReceive();
            }
        }
    );
}

void UdpSocket::setOnReceive(ReceiveHandler handler)
{
    _receiveHandler = std::move(handler);
}

void UdpSocket::setOnSend(SendHandler handler)
{
    _sendHandler = std::move(handler);
}

std::error_code UdpSocket::_toStdError(const boost::system::error_code& boostEc)
{
    return std::error_code(boostEc.value(), std::generic_category());
}
