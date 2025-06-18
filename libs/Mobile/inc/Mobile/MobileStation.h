#ifndef _MOBILE_MOBILE_STATION_H
#define _MOBILE_MOBILE_STATION_H

#include <functional>
#include <cstdint>

#include <Network/GsmMessage.h>
#include <Network/MobileIdentity.h>

#include <Network/RadioLink.h>

class MobileStation {
public:
    using VoiceFrame = std::vector<uint8_t>;

    enum class State {
        DISCONNECTED,
        IDLE,
        AUTHENTICATING,
        CIPHERING,
        CALL_IDLE,
        IN_CALL
    };

    MobileStation(RadioLink& link, const MobileIdentity& imsi, const std::vector<uint8_t>& ki);

    State state() const { return _state; }
    const std::vector<uint8_t>& lai() const;

    void connectToBts();
    void disconnectFromBts();
    void sendVoiceData(const VoiceFrame& speech);

    void setLai(const std::vector<uint8_t>& lai);

    void setReceiveVoiceFrameHandler(std::function<void(const VoiceFrame&)> cb);

private:
    RadioLink& _link;
    MobileIdentity _imsi;
    std::vector<uint8_t> _ki;

    State _state;
    std::vector<uint8_t> _kc;
    uint8_t _keySeq;
    uint8_t _transactionId;
    std::vector<uint8_t> _lai;

    std::function<void(const VoiceFrame&)> _voiceCb;

    void _handleAuthRequest(const GsmMessage& msg);
    void _handleCipherModeCommand(const GsmMessage& msg);
    void _handleSetup(const GsmMessage& msg);
    void _handleConnectAcknowledge(const GsmMessage& msg);
    void _handleVoiceFrame(const GsmMessage& msg);

    void _sendAuthResponse(const std::vector<uint8_t>& rand);
    void _sendSetupRequest();
};

#endif
