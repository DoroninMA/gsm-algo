#include <Network/UdpSocket.h>

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
    _socket.send_to(boost::asio::buffer(data), _remoteEndpoint);
}

void UdpSocket::asyncSendRequest(const std::vector<uint8_t>& data)
{
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
