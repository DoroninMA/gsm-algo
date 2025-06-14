#include <Network/UdpSocket.h>

const std::size_t UdpSocket::DEFAULT_BUFFER_SIZE = 2048;

UdpSocket::UdpSocket(boost::asio::io_context& ioContext, uint16_t localPort, std::size_t bufferSize)
    : _ioContext(ioContext),
      _socket(ioContext, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), localPort)),
      _recvBuffer(bufferSize),
      _remoteEndpoint(boost::asio::ip::udp::v4(), 0)
{
}

UdpSocket::~UdpSocket()
{
    try
    {
        if (_socket.is_open())
            _socket.close();
    }
    catch (...) {}
}

std::string UdpSocket::getLastSenderAddress() const
{
    return _lastSenderEndpoint.address().to_string();
}

uint16_t UdpSocket::getLastSenderPort() const
{
    return _lastSenderEndpoint.port();
}

void UdpSocket::setRemote(const std::string& remoteAddress, uint16_t remotePort)
{
    _remoteEndpoint = boost::asio::ip::udp::endpoint(
        boost::asio::ip::address::from_string(remoteAddress), remotePort);
}

void UdpSocket::send(const std::vector<uint8_t>& data)
{
    if (_remoteEndpoint.address().is_unspecified())
    {
        std::cerr << "Remote endpoint is not set!" << std::endl;
        return;
    }

    try
    {
        _socket.send_to(boost::asio::buffer(data), _remoteEndpoint);
    }
    catch (const boost::system::system_error& e)
    {
        std::cerr << "Error sending UDP message: " << e.what() << std::endl;
    }
}

void UdpSocket::asyncSend(const std::vector<uint8_t>& data)
{
    if (_remoteEndpoint.address().is_unspecified())
    {
        if (_sendHandler)
        {
            boost::system::error_code ec = boost::asio::error::invalid_argument;
            _sendHandler(ec, 0);
        }
        return;
    }

    _socket.async_send_to(
        boost::asio::buffer(data),
        _remoteEndpoint,
        [this](const boost::system::error_code& ec, std::size_t bytesSent)
        {
            if (_sendHandler)
                _sendHandler(ec, bytesSent);
        });
}

std::vector<uint8_t> UdpSocket::receive()
{
    std::vector<uint8_t> buffer(_recvBuffer.size());

    try
    {
        _lastSenderEndpoint = boost::asio::ip::udp::endpoint();

        size_t bytesReceived = _socket.receive_from(
            boost::asio::buffer(buffer), _lastSenderEndpoint);

        buffer.resize(bytesReceived);
    }
    catch (const boost::system::system_error& e)
    {
        std::cerr << "Error receiving UDP message: " << e.what() << std::endl;
        buffer.clear();
    }

    return buffer;
}

void UdpSocket::asyncReceive()
{
    _recvBuffer.resize(DEFAULT_BUFFER_SIZE);

    _socket.async_receive_from(
        boost::asio::buffer(_recvBuffer),
        _lastSenderEndpoint,
        [this](const boost::system::error_code& ec, std::size_t bytesReceived)
        {
            std::vector<uint8_t> data;
            if (!ec)
            {
                data.assign(_recvBuffer.begin(), _recvBuffer.begin() + bytesReceived);
            }

            std::string senderAddr = _lastSenderEndpoint.address().to_string();
            uint16_t senderPort = _lastSenderEndpoint.port();

            if (_receiveHandler)
            {
                _receiveHandler(ec, bytesReceived, data, senderAddr, senderPort);
            }
        });
}

void UdpSocket::setOnReceive(ReceiveHandler handler)
{
    _receiveHandler = std::move(handler);
}

void UdpSocket::setOnSend(SendHandler handler)
{
    _sendHandler = std::move(handler);
}
