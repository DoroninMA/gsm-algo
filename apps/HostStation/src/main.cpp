#include <boost/asio/io_context.hpp>

#include "GsmCrypto/Auth/AuthComp1281.h"
#include "GsmCrypto/Auth/AuthGenerator.h"
#include "GsmCrypto/Encrypt/EcnryptMethod.h"
#include "Host/Host.h"
#include "Network/RadioLink.h"
#include "Network/UdpSocket.h"

#include "GsmCrypto/Encrypt/EcnryptA51.h"

int main()
{
    try
    {
        boost::asio::io_context io_context;

        uint16_t localPort = 12345;
        auto udpSocket = std::make_shared<UdpSocket>(io_context, localPort);

        RadioLink radioLink(udpSocket);
        // std::vector<uint8_t> lai = {0x21, 0x43, 0x65};
        std::vector<uint8_t> lai = {0, 1, 0, 1, 18};

        Host host(radioLink, lai);

        std::unique_ptr<EncryptMethod> encryptMethod = std::make_unique<EncryptA51>();
        std::unique_ptr<AuthGenerator> authGen = std::make_unique<Comp1281>();

        host.setEncryptMethod(std::move(encryptMethod));
        host.setAuthGenerator(std::move(authGen));

        udpSocket->asyncReceive();

        std::cout << "HostStation running on UDP port " << localPort << "...\n";
        io_context.run();

    }
    catch (const std::exception& ex)
    {
        std::cerr << "Exception in HostStation: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
