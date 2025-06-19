#include <iostream>
#include <random>
#include <Host/Host.h>

#include "Network/Level3/CcMessage/ConnectAck.h"
#include "Network/Level3/MmMessage/AuthRequestMessage.h"
#include "Network/Level3/MmMessage/AuthResponse.h"
#include "Network/Level3/MmMessage/CipherModeCommand.h"
#include "Network/Level3/MmMessage/LocationUpdateRequest.h"
#include "Network/Level3/L3MessageFactory.h"

Host::Host(RadioLink& link) : _link(link)
{
    _link.setReceiveHandler([this](const std::error_code& ec, std::size_t, const std::vector<uint8_t>& data,
                                   const std::string&, uint16_t)
    {
        if (ec)
            return;
        try
        {
            std::unique_ptr<GsmMessage> msg = MessageFactory::parse(data);
            switch (msg->messageType())
            {
                case GsmMsgTypeL3::LOCATION_UPDATE_REQUEST:
                    _handleLocationUpdateRequest(*msg);
                    break;
                case GsmMsgTypeL3::AUTH_RESPONSE:
                    _handleAuthResponse(*msg);
                    break;
                case GsmMsgTypeL3::CIPHER_MODE_COMPLETE:
                    _handleCipherModeComplete(*msg);
                    break;
                case GsmMsgTypeL3::SETUP:
                    _handleSetup(*msg);
                    break;
                case GsmMsgTypeL3::RELEASE:
                    _handleRelease(*msg);
                    break;
                case GsmMsgTypeL3::VOICE_FRAME:
                    _handleVoiceFrame(*msg);
                    break;
                default:
                    std::cerr << "Host: unknown message type received\n";
                    break;
            }
        }
        catch (const std::exception& ex)
        {
            std::cerr << "Host: exception while processing message: " << ex.what() << "\n";
        }
    });
}

void Host::setEncryptMethod(std::unique_ptr<EncryptMethod> encryptMethod)
{
    _pEncrypt = std::move(encryptMethod);
    if (_pEncrypt)
        _pEncrypt->setFrameNumber(0);
}

void Host::setAuthGenerator(std::unique_ptr<AuthGenerator> authGen)
{
    _pAuthGen = std::move(authGen);
}

void Host::setVoiceHandler(std::function<void(const VoiceFrame&)> cb)
{
    _voiceCb = std::move(cb);
}

void Host::_handleLocationUpdateRequest(const GsmMessage& msg)
{
    if (!_pAuthGen)
    {
        throw std::runtime_error("Host: AuthGenerator not set");
    }

    const auto& lur = static_cast<const LocationUpdateRequest&>(msg);

    std::vector<uint8_t> rand(16);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 255);
    for (auto& b: rand)
        b = static_cast<uint8_t>(dist(gen));

    AuthRequestMessage auth;
    auth.setRand(rand);

    _pAuthGen->setRand(rand);
    _pAuthGen->generateNext(_expectedSres, _kc);

    _link.send(auth.pack());
}

void Host::_handleAuthResponse(const GsmMessage& msg)
{
    if (!_pAuthGen)
    {
        throw std::runtime_error("Host: AuthGenerator not set");
    }

    const auto& authResp = static_cast<const AuthResponseMessage&>(msg);
    if (authResp.sres() != _expectedSres)
    {
        std::cerr << "Host: authentication failed\n";
        return;
    }

    CipherModeCommand cmd;
    cmd.setCipherAlgorithm(1); // Example algorithm ID
    cmd.setKeySequence(_keySeq);
    _link.send(cmd.pack());
}

void Host::_handleCipherModeComplete(const GsmMessage&)
{
    if (_pEncrypt)
    {
        _pEncrypt->setKc(_kc);
        _pEncrypt->setFrameNumber(0);
    }
    else
    {
        std::cerr << "Host: no EncryptMethod set during CipherModeComplete\n";
    }
}

void Host::_handleSetup(const GsmMessage& msg)
{
    ConnectAck ack;
    _link.send(ack.pack());
}

void Host::_handleRelease(const GsmMessage&)
{
    std::cout << "Host: received RELEASE, ending call/session.\n";
}

void Host::_handleVoiceFrame(const GsmMessage& msg)
{
    if (!_pEncrypt)
    {
        std::cerr << "Host: no encryption method available for VoiceFrame\n";
        return;
    }

    const std::vector<uint8_t>& encrypted = msg.pack();
    std::vector<uint8_t> decrypted = _pEncrypt->decrypt(encrypted);
    _pEncrypt->setFrameNumber(_pEncrypt->frameNumber() + 1);

    if (_voiceCb)
    {
        _voiceCb(decrypted);
    }
}
