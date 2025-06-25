#include <Mobile/MobileStation.h>

#include <iostream>
#include <stdexcept>
#include <utility>

#include <GsmCrypto/Encrypt/EcnryptA51.h>
#include <GsmCrypto/Encrypt/EcnryptA52.h>

#include <GsmCrypto/Auth/AuthComp1281.h>
#include <GsmCrypto/Auth/AuthComp1282.h>
#include <GsmCrypto/Auth/AuthComp1283.h>

#include "GsmCrypto/Encrypt/EncryptMethodFactory.h"
#include "Network/Level3/L3MessageFactory.h"
#include "Network/Level3/CcMessage/ReleaseMessage.h"
#include "Network/Level3/CcMessage/SetupMessage.h"
#include "Network/Level3/MmMessage/AuthRequestMessage.h"
#include "Network/Level3/MmMessage/AuthResponse.h"
#include "Network/Level3/MmMessage/CipherModeCommand.h"
#include "Network/Level3/MmMessage/CipherModeComplete.h"
#include "Network/Level3/MmMessage/LocationUpdateRequest.h"

#include "IOUtils/Utils.h"
#include "Network/Level2/VoiceMessage.h"

MobileStation::MobileStation(RadioLink& link, const MobileIdentity& imsi, const std::vector<uint8_t>& ki)
    : _link(link), _imsi(imsi), _ki(ki), _state(State::DISCONNECTED),
      _keySeq(0), _transactionId(0)
{
    _link.setReceiveHandler([this](
        const std::error_code& ec,
        std::size_t bytesReceived,
        const std::vector<uint8_t>& raw,
        const std::string& senderAddress,
        uint16_t senderPort)
    {
        if (ec.value()) return;

        try
        {
            std::unique_ptr<GsmMessage> msg = std::move(MessageFactory::parse(raw));
            if (msg->protocolDiscriminator() == GsmMessagePD::TRAFIC_CHANNEL)
            {
                _handleVoiceFrame(*msg);
            }
            else
            {
                switch (msg->messageType())
                {
                    case static_cast<uint8_t>(GsmMsgTypeMM::AUTH_REQUEST):
                        _handleAuthRequest(*msg);
                        break;
                    case static_cast<uint8_t>(GsmMsgTypeMM::LOCATION_UPDATE_REJECT):
                        _handleAuthReject(*msg);
                        break;
                    case static_cast<uint8_t>(GsmMsgTypeMM::CIPHER_MODE_COMMAND):
                        _handleCipherModeCommand(*msg);
                        break;
                    // case static_cast<uint8_t>(GsmMsgTypeMM::SETUP):
                    //     _handleSetup(*msg);
                    //     break;
                    case static_cast<uint8_t>(GsmMsgTypeCC::CONNECT_ACK):
                        _handleConnectAcknowledge(*msg);
                        break;
                    default:
                        break;
                }
            }
        }
        catch (const std::exception& ex)
        {
            std::cerr << "MobileStation: Exception in message handler: " << ex.what() << "\n";
        }
    });
}

const std::vector<uint8_t>& MobileStation::lai() const
{
    return _lai;
}

int MobileStation::alogId() const
{
    if (_pEncryptMethod)
    {
        if (dynamic_cast<EncryptA51*>(_pEncryptMethod.get()))
        {
            return 1;
        }

        if (dynamic_cast<EncryptA52*>(_pEncryptMethod.get()))
        {
            return 2;
        }
    }

    return 0;
}

int MobileStation::authId() const
{
    if (_pAuthGenerator)
    {
        if (dynamic_cast<Comp1281*>(_pAuthGenerator.get()))
        {
            return 1;
        }

        if (dynamic_cast<Comp1282*>(_pAuthGenerator.get()))
        {
            return 2;
        }

        if (dynamic_cast<Comp1283*>(_pAuthGenerator.get()))
        {
            return 3;
        }
    }

    return 0;
}

const std::vector<uint8_t> &MobileStation::kc() const
{
    return _kc;
}

const std::vector<uint8_t> &MobileStation::rand() const
{
    static std::vector<uint8_t> emptyRand{};

    if (_pAuthGenerator)
    {
        return _pAuthGenerator->rand();
    }

    return emptyRand;
}

void MobileStation::connectToBts()
{
    if (_state != State::DISCONNECTED)
    {
        throw std::runtime_error("Already connected or in progress");
    }

    _state = State::AUTHENTICATING;

    LocationUpdateRequest lur;
    lur.setLai(_lai);
    lur.setMobileIdentity(_imsi);
    _link.sendRequest(lur.pack());
}

std::unique_ptr<EncryptMethod> MobileStation::_createEncryptMethod(uint8_t methodId)
{
    return EncryptMethodFactory::create(methodId);
}

void MobileStation::_handleAuthRequest(const GsmMessage& msg)
{
    if (_state != State::AUTHENTICATING)
    {
        std::cerr << "MobileStation: Unexpected AUTH_REQUEST in state " << static_cast<int>(_state) << "\n";
        return;
    }

    const auto& authReq = static_cast<const AuthRequestMessage&>(msg);
    std::cout << "LUR Response. Rand: " << _bytesToHexString(authReq.rand().data(), authReq.rand().size()) << std::endl;
    _sendAuthResponse(authReq.rand());
    _state = State::CIPHERING;
}

void MobileStation::_sendAuthResponse(const std::vector<uint8_t>& rand)
{
    if (_pAuthGenerator == nullptr)
    {
        throw std::runtime_error("MobileStation: set AuthGenerator before call authentication");
    }

    std::vector<uint8_t> sres;
    _pAuthGenerator->setKi(_ki);
    _pAuthGenerator->setRand(rand);
    _pAuthGenerator->generateNext(sres, _kc);

    std::cout << "====== AuthResponseMessage ======" << std::endl;
    std::cout << "ki: " << _bytesToHexString(_ki.data(), _ki.size()) << std::endl;
    std::cout << "rand: " << _bytesToHexString(rand.data(), rand.size()) << std::endl;
    std::cout << "sres: " << _bytesToHexString(sres.data(), sres.size()) << std::endl;
    std::cout << "kc: " << _bytesToHexString(_kc.data(), _kc.size()) << std::endl;
    std::cout << "=================================" << std::endl;

    AuthResponseMessage resp;
    resp.setSres(sres);
    _link.sendRequest(resp.pack());
}

void MobileStation::_handleAuthReject(const GsmMessage& msg)
{
    if (_state != State::AUTHENTICATING)
    {
        std::cerr << "MobileStation: Unexpected LOCATION_UPDATE_REJECT in state " << static_cast<int>(_state) << "\n";
        return;
    }

    std::cerr << "MobileStation: Authentication rejected by network\n";
    _state = State::DISCONNECTED;
    _kc.clear();
    _keySeq = 0;
}

void MobileStation::_handleCipherModeCommand(const GsmMessage& msg)
{
    auto& cmcRequest = static_cast<const CipherModeCommand&>(msg);
    uint8_t algoId = cmcRequest.cipherAlgorithm();
    _keySeq = cmcRequest.keySequence();

    // update encryption method
    _pEncryptMethod = _createEncryptMethod(algoId);
    if (_pEncryptMethod == nullptr)
    {
        throw std::runtime_error("MobileStation: EncryptMethod not defined");
    }
    _pEncryptMethod->setKc(_kc);
    _pEncryptMethod->setFrameNumber(0);

    std::cout << "========== RECV CMC ==============" << std::endl;
    std::cout << "ciper algorithm: " << static_cast<int>(algoId) << std::endl;
    std::cout << "key sequence: " << static_cast<int>(_keySeq) << std::endl;
    std::cout << "==================================" << std::endl;

    std::cout << "Send CMC response ok" << std::endl;
    // send response
    CipherModeComplete cmcResponse;
    cmcResponse.setCryptoAlgoId(algoId);
    _link.sendRequest(cmcResponse.pack());

    _state = State::CALL_IDLE;
    _sendSetupRequest();
}

void MobileStation::_sendSetupRequest()
{
    // 0x11: ITC=Speech, CS=ITU-T
    // 0x22: Transfer mode=CS (Circuit Switched), Transfer rate=9.6 kbps
    SetupMessage setup({0x11, 0x22}, _imsi.toString(), "");
    _link.sendRequest(setup.pack());
    _state = State::IN_CALL;
}

// void MobileStation::_handleSetup(const GsmMessage& msg)
// {
// }

void MobileStation::_handleConnectAcknowledge(const GsmMessage& msg)
{
    _state = State::IN_CALL;
}

void MobileStation::disconnectFromBts()
{
    if (_state == State::DISCONNECTED || _state == State::IDLE)
    {
        return;
    }

    ReleaseMessage releaseMsg(16); // normal release cause
    releaseMsg.setTransactionId(_transactionId);
    _link.sendRequest(releaseMsg.pack());

    _state = State::IDLE;
}

void MobileStation::sendVoiceData(const std::string &speech)
{
    std::vector<uint8_t> msg(speech.size());
    std::copy(speech.cbegin(), speech.cend(), msg.begin());
    sendVoiceData(msg);
}

void MobileStation::sendVoiceData(const std::vector<uint8_t>& speech)
{
    if (_state != State::IN_CALL)
    {
        throw std::runtime_error("MobileStation: Not in call");
    }

    if (!_pEncryptMethod)
    {
        throw std::runtime_error("MobileStation: No encryption method set");
    }

    // encode message
    std::vector<uint8_t> encrypted = _pEncryptMethod->encrypt(speech);
    std::cout << "======= RECEIVE VOICE ==========" << std::endl;
    std::cout << "Input: " << _bytesToHexString(speech.data(), speech.size()) << std::endl;
    std::cout << "Encrypted: " << _bytesToHexString(encrypted.data(), encrypted.size()) << std::endl;
    std::cout << "KC: " << _bytesToHexString(_pEncryptMethod->kc().data(), _pEncryptMethod->kc().size()) << std::endl;
    std::cout << "Fn: " << _pEncryptMethod->frameNumber() << std::endl;
    std::cout << "================================" << std::endl;

    VoiceMessage voiceMsg(encrypted);
    _link.sendRequest(voiceMsg.pack());
}

void MobileStation::setLai(const std::vector<uint8_t>& lai)
{
    _lai = lai;
}

void MobileStation::setReceiveVoiceFrameHandler(std::function<void(const VoiceFrame&)> cb)
{
    _voiceCb = std::move(cb);
}

void MobileStation::setAuthGenerator(std::unique_ptr<AuthGenerator>&& pAuthGenerator)
{
    _pAuthGenerator = std::move(pAuthGenerator);
}

void MobileStation::_handleVoiceFrame(const GsmMessage& msg)
{
    if (!_pEncryptMethod)
    {
        throw std::runtime_error("MobileStation: No encryption method set");
    }

    auto& voice = static_cast<const VoiceMessage&>(msg);

    // decode message
    std::vector<uint8_t> decrypted = _pEncryptMethod->decrypt(voice.voiceData());

    if (_voiceCb)
    {
        _voiceCb(decrypted);
    }
}
