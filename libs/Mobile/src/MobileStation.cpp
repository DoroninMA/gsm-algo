#include <iostream>
#include <Mobile/MobileStation.h>

#include <stdexcept>
#include <utility>

#include <Network/Level3/L3MessageFactory.h>

#include "Network/Level3/CcMessage/ReleaseMessage.h"
#include "Network/Level3/CcMessage/SetupMessage.h"
#include "Network/Level3/MmMessage/AuthRequestMessage.h"
#include "Network/Level3/MmMessage/AuthResponse.h"
#include "Network/Level3/MmMessage/CipherModeCommand.h"
#include "Network/Level3/MmMessage/CipherModeComplete.h"
#include "Network/Level3/MmMessage/LocationUpdateRequest.h"


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
            std::unique_ptr<GsmMessage> msg = MessageFactory::parse(raw);
            switch (msg->messageType())
            {
                case GsmMsgTypeMM::AUTH_REQUEST:
                    _handleAuthRequest(*msg);
                    break;
                case GsmMsgTypeMM::LOCATION_UPDATE_REJECT:
                    _handleAuthReject(*msg);
                    break;
                case GsmMsgTypeMM::CIPHER_MODE_COMMAND:
                    _handleCipherModeCommand(*msg);
                    break;
                // case GsmMsgTypeL3::SETUP:
                //     _handleSetup(*msg);
                //     break;
                case GsmMsgTypeCC::CONNECT_ACK:
                    _handleConnectAcknowledge(*msg);
                    break;
                case GsmMsgTypeCC::VOICE_FRAME:
                    _handleVoiceFrame(*msg);
                    break;
                default:
                    break;
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
    _link.send(lur.pack());
}

std::unique_ptr<EncryptMethod> MobileStation::_createEncryptMethod(uint8_t methodId)
{
    switch (methodId)
    {
        case
    }

    // @todo add resolving by id
    return nullptr;
}

void MobileStation::_handleAuthRequest(const GsmMessage& msg)
{
    if (_state != State::AUTHENTICATING)
    {
        std::cerr << "MobileStation: Unexpected AUTH_REQUEST in state " << static_cast<int>(_state) << "\n";
        return;
    }

    const auto& authReq = static_cast<const AuthRequestMessage&>(msg);
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

    AuthResponseMessage resp;
    resp.setSres(sres);
    _link.send(resp.pack());
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

    // send response
    CipherModeComplete cmcResponse;
    _link.send(cmcResponse.pack());

    _state = State::CALL_IDLE;
    _sendSetupRequest();
}

void MobileStation::_sendSetupRequest()
{
    // 0x11: ITC=Speech, CS=ITU-T
    // 0x22: Transfer mode=CS (Circuit Switched), Transfer rate=9.6 kbps
    SetupMessage setup({0x11, 0x22}, _imsi.toString(), "");
    _link.send(setup.pack());
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
    _link.send(releaseMsg.pack());

    _state = State::IDLE;
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
    _pEncryptMethod->setFrameNumber(_pEncryptMethod->frameNumber() + 1);

    _link.send(encrypted);
}

void MobileStation::setLai(const std::vector<uint8_t>& lai)
{
    _lai = lai;
}

void MobileStation::setReceiveVoiceFrameHandler(std::function<void(const VoiceFrame&)> cb)
{
    _voiceCb = std::move(cb);
}

void MobileStation::setAuthGenerator(std::unique_ptr<AuthGenerator>& pAuthGenerator)
{
    _pAuthGenerator = std::move(pAuthGenerator);
}

void MobileStation::_handleVoiceFrame(const GsmMessage& msg)
{
    if (!_pEncryptMethod)
    {
        throw std::runtime_error("MobileStation: No encryption method set");
    }

    // decode message
    std::vector<uint8_t> encrypted = msg.pack();
    std::vector<uint8_t> decrypted = _pEncryptMethod->decrypt(encrypted);
    _pEncryptMethod->setFrameNumber(_pEncryptMethod->frameNumber() + 1);

    if (_voiceCb)
    {
        _voiceCb(decrypted);
    }
}
