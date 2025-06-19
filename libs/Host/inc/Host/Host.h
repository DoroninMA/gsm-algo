#ifndef HOST_HOST_H
#define HOST_HOST_H

#include <vector>

#include "GsmCrypto/Auth/AuthGenerator.h"
#include "GsmCrypto/Encrypt/EcnryptMethod.h"
#include "Network/GsmMessage.h"
#include "Network/RadioLink.h"

class Host
{
public:
    using VoiceFrame = std::vector<uint8_t>;

    explicit Host(RadioLink& link);

    void setEncryptMethod(std::unique_ptr<EncryptMethod> encryptMethod);
    void setAuthGenerator(std::unique_ptr<AuthGenerator> authGen);
    void setVoiceHandler(std::function<void(const VoiceFrame&)> cb);

private:
    RadioLink& _link;
    std::vector<uint8_t> _kc;
    std::vector<uint8_t> _expectedSres;

    uint8_t _keySeq = 0;

    std::unique_ptr<AuthGenerator> _pAuthGen;
    std::unique_ptr<EncryptMethod> _pEncrypt;

    std::function<void(const VoiceFrame&)> _voiceCb;

    void _handleLocationUpdateRequest(const GsmMessage& msg);
    void _handleAuthResponse(const GsmMessage& msg);
    void _handleCipherModeComplete(const GsmMessage& msg);
    void _handleSetup(const GsmMessage& msg);
    void _handleRelease(const GsmMessage& msg);
    void _handleVoiceFrame(const GsmMessage& msg);
};


#endif
