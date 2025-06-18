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
    _link.setReceiveHandler([this](const std::vector<uint8_t>& raw)
    {
        std::unique_ptr<GsmMessage> msg = MessageFactory::parse(raw);
        switch (msg->messageType())
        {
            case GsmMsgTypeL3::AUTH_REQUEST:
                _handleAuthRequest(*msg);
                break;
            case GsmMsgTypeL3::CIPHER_MODE_COMMAND:
                _handleCipherModeCommand(*msg);
                break;
            // case GsmMsgTypeL3::SETUP:
            //     _handleSetup(*msg);
            //     break;
            case GsmMsgTypeL3::CONNECT_ACK:
                _handleConnectAcknowledge(*msg);
                break;
            case GsmMsgTypeL3::VOICE_FRAME:
                _handleVoiceFrame(*msg);
                break;
            default:
                break;
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

void MobileStation::_handleAuthRequest(const GsmMessage& msg)
{
    const auto& authReq = static_cast<const AuthRequestMessage&>(msg);
    _sendAuthResponse(authReq.rand());
    _state = State::CIPHERING;
}

void MobileStation::_sendAuthResponse(const std::vector<uint8_t>& rand)
{
    std::vector<uint8_t> sres = cryptoA3(_ki, rand);
    _kc = cryptoA8(_ki, rand);

    AuthResponseMessage resp;
    resp.setSres(sres);
    _link.send(resp.pack());
}

void MobileStation::_handleCipherModeCommand(const GsmMessage& msg)
{
    auto& cmcRequest = static_cast<const CipherModeCommand&>(msg);
    _keySeq = cmcRequest.keySequence();

    CipherModeComplete cmcResponse;
    _link.send(cmcResponse.pack());

    _state = State::CALL_IDLE;
    _sendSetupRequest();
}

void MobileStation::_sendSetupRequest()
{
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

    _link.send(speech);
}

void MobileStation::setLai(const std::vector<uint8_t>& lai)
{
    _lai = lai;
}

void MobileStation::setReceiveVoiceFrameHandler(std::function<void(const VoiceFrame&)> cb)
{
    _voiceCb = std::move(cb);
}

void MobileStation::_handleVoiceFrame(const GsmMessage& msg)
{
    if (_voiceCb)
    {
        _voiceCb(msg.pack());
    }
}
