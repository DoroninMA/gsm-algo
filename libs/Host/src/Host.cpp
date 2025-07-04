#include <iostream>
#include <random>
#include <bits/regex_constants.h>
#include <Host/Host.h>

#include "Network/Level2/VoiceMessage.h"
#include "Network/Level3/CcMessage/ConnectAck.h"
#include "Network/Level3/MmMessage/AuthRequestMessage.h"
#include "Network/Level3/MmMessage/AuthResponse.h"
#include "Network/Level3/MmMessage/CipherModeCommand.h"
#include "Network/Level3/MmMessage/LocationUpdateRequest.h"
#include "Network/Level3/L3MessageFactory.h"
#include "Network/Level3/CcMessage/ReleaseCompleteMessage.h"
#include "Network/Level3/CcMessage/ReleaseMessage.h"
#include "Network/Level3/MmMessage/CipherModeComplete.h"
#include "Network/Level3/MmMessage/LocationUpdateAccept.h"
#include "Network/Level3/MmMessage/LocationUpdateReject.h"

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

Host::Host(RadioLink& link) : _link(link)
{
    _link.setReceiveHandler([this](const std::error_code& ec, std::size_t, const std::vector<uint8_t>& data,
                                   const std::string&, uint16_t)
    {
        if (ec)
            return;
        try
        {
            std::string debugMessage = _bytesToHexString(data.data(), data.size());
            std::cout << "received: " << debugMessage << std::endl;

            std::unique_ptr<GsmMessage> msg = MessageFactory::parse(data);
            if (msg->protocolDiscriminator() == GsmMessagePD::TRAFIC_CHANNEL)
            {
                _handleVoiceFrame(*msg);
            }
            else
            {
                switch (msg->messageType())
                {
                    case static_cast<uint8_t>(GsmMsgTypeMM::LOCATION_UPDATE_REQUEST):
                        _handleLocationUpdateRequest(*msg);
                        break;
                    case static_cast<uint8_t>(GsmMsgTypeMM::AUTH_RESPONSE):
                        _handleAuthResponse(*msg);
                        break;
                    case static_cast<uint8_t>(GsmMsgTypeMM::CIPHER_MODE_COMPLETE):
                        _handleCipherModeComplete(*msg);
                        break;
                    case static_cast<uint8_t>(GsmMsgTypeCC::SETUP):
                        _handleSetup(*msg);
                        break;
                    case static_cast<uint8_t>(GsmMsgTypeCC::RELEASE):
                        _handleRelease(*msg);
                        break;
                    default:
                        std::cerr << "Host: unknown message type received\n";
                        break;
                }
            }
        }
        catch (const std::exception& ex)
        {
            std::cerr << "Host: exception while processing message: " << ex.what() << "\n";
        }
    });
}

Host::Host(RadioLink& link, const std::vector<uint8_t>& lai) : Host(link)
{
    setLai(lai);
}

const std::vector<uint8_t>& Host::lai() const
{
    return _lai;
}

void Host::setLai(const std::vector<uint8_t>& lai)
{
    _lai = lai;
}

void Host::setEncryptMethod(std::unique_ptr<EncryptMethod> encryptMethod)
{
    _pEncrypt = std::move(encryptMethod);
    if (_pEncrypt)
    {
        _pEncrypt->setFrameNumber(0);
    }
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

    std::cout << "=============== LUR =============" << std::endl;

    const auto& lur = static_cast<const LocationUpdateRequest&>(msg);
    if (lur.lai() != _lai)
    {
        std::cerr << "Host: Location area not allowed\n";

        LocationUpdateReject rejectMsg;
        rejectMsg.setCause(static_cast<uint8_t>(GsmLurCause::LA_NOT_ALLOWED));
        _link.sendResponse(rejectMsg.pack());

        std::cout << "=================================" << std::endl;
        return;
    }

    std::vector<uint8_t> rand(16);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 255);
    for (uint8_t& b: rand)
    {
        b = static_cast<uint8_t>(dist(gen));
    }

    AuthRequestMessage auth;
    auth.setRand(rand);

    /// @todo add ki finding
    std::vector<uint8_t> ki(16, 0x00);

    _pAuthGen->setKi(ki);
    _pAuthGen->setRand(rand);
    _pAuthGen->generateNext(_expectedSres, _kc);

    std::cout << "LUR accept. " << std::endl;
    std::cout << "Ki: " << _bytesToHexString(ki.data(), ki.size()) << std::endl;
    std::cout << "Rand: " << _bytesToHexString(rand.data(), rand.size()) << std::endl;
    std::cout << "XRES: " << _bytesToHexString(_expectedSres.data(), _expectedSres.size()) << std::endl;
    std::cout << "Kc: " << _bytesToHexString(_kc.data(), _kc.size()) << std::endl;

    std::cout << "=================================" << std::endl;

    _link.sendResponse(auth.pack());
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

        LocationUpdateReject rejectMsg;
        rejectMsg.setCause(static_cast<uint8_t>(GsmLurCause::AUTH_FAIL));
        _link.sendResponse(rejectMsg.pack());

        return;
    }

    std::cout << "LUR accept" << std::endl;
    LocationUpdateAccept acceptMsg;
    acceptMsg.setLai(lai());
    _link.sendResponse(acceptMsg.pack());

    CipherModeCommand cmd;
    cmd.setCipherAlgorithm(1);
    cmd.setKeySequence(_keySeq);

    std::cout << "========== SEND CMC ==============" << std::endl;
    std::cout << "ciper algorithm: " << static_cast<int>(cmd.cipherAlgorithm()) << std::endl;
    std::cout << "key sequence: " << static_cast<int>(cmd.keySequence()) << std::endl;
    std::cout << "=================================" << std::endl;

    _link.sendResponse(cmd.pack());
}

void Host::_handleCipherModeComplete(const GsmMessage& msg)
{
    const auto& cmcMsg = static_cast<const CipherModeComplete&>(msg);

    std::cout << "Cipher mode complete recv";
    if (cmcMsg.isCryptoAlgoIdExist())
    {
        std::cout << ": algo id " << static_cast<int>(cmcMsg.cryptoAlgoId());
    }
    std::cout << std::endl;

    if (_pEncrypt)
    {
        _pEncrypt->reset();
        _pEncrypt->setKc(_kc);
    }
    else
    {
        std::cerr << "Host: no EncryptMethod set during CipherModeComplete\n";
    }
}

void Host::_handleSetup(const GsmMessage& msg)
{
    ConnectAck ack;
    _link.sendResponse(ack.pack());
}

void Host::_handleRelease(const GsmMessage& msg)
{
    const auto& release = static_cast<const ReleaseMessage&>(msg);

    ReleaseCompleteMessage releaseComplete;
    releaseComplete.setTransactionId(release.transactionId());

    _link.sendResponse(releaseComplete.pack());
}

void Host::_handleVoiceFrame(const GsmMessage& msg)
{
    if (!_pEncrypt)
    {
        std::cerr << "Host: no encryption method available for VoiceFrame\n";
        return;
    }

    auto& voice = static_cast<const VoiceMessage&>(msg);
    const std::vector<uint8_t> decrypted = _pEncrypt->encrypt(voice.voiceData());

    const auto& voiceData = voice.voiceData();
    std::cout << "======= RECEIVE VOICE ==========" << std::endl;
    std::cout << "Encrypted: " << _bytesToHexString(voiceData.data(), voiceData.size()) << std::endl;
    std::cout << "Decrypted: " << _bytesToHexString(decrypted.data(), decrypted.size()) << std::endl;
    std::cout << "Text: ";
    for (char i : decrypted)
    {
        std::cout << i;
    }
    std::cout << std::endl;
    std::cout << "KC: " << _bytesToHexString(_pEncrypt->kc().data(), _pEncrypt->kc().size()) << std::endl;
    std::cout << "Fn: " << _pEncrypt->frameNumber() << std::endl;
    std::cout << "================================" << std::endl;

    if (_voiceCb)
    {
        _voiceCb(decrypted);
    }
}
