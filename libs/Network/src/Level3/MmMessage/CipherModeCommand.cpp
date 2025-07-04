#include <Network/Level3/MmMessage/CipherModeCommand.h>

#include <iostream>
#include <stdexcept>
#include "Network/GsmDefs.h"

static constexpr uint8_t TLV_MI_TAG = 0x22; // Mobile Identity

CipherModeCommand::CipherModeCommand()
    : _cipherAlgorithm(0), _keySequence(0), _mobileIdentity(TLV_MI_TAG, {}), _isMobileIdentityExist(false)
{
}

uint8_t CipherModeCommand::messageType() const
{
    return static_cast<uint8_t>(GsmMsgTypeMM::CIPHER_MODE_COMMAND);
}

uint8_t CipherModeCommand::cipherAlgorithm() const
{
    return _cipherAlgorithm;
}

uint8_t CipherModeCommand::keySequence() const
{
    return _keySequence;
}

std::vector<uint8_t> CipherModeCommand::mobileIdentity() const
{
    return _mobileIdentity.value();
}

bool CipherModeCommand::isMobileIdentityExist() const
{
    return _isMobileIdentityExist;
}

void CipherModeCommand::setCipherAlgorithm(uint8_t algo)
{
    _cipherAlgorithm = algo;
}

void CipherModeCommand::setKeySequence(uint8_t seq)
{
    _keySequence = seq;
}

void CipherModeCommand::setMobileIdentity(const std::vector<uint8_t>& mi)
{
    if (mi.empty())
    {
        throw std::invalid_argument("CipherModeCommand: Mobile Identity cannot be empty");
    }

    _mobileIdentity = Tlv(TLV_MI_TAG, mi);
    _isMobileIdentityExist = true;
}

void CipherModeCommand::parse(const std::vector<uint8_t>& data)
{
    std::cout << "CipherModeCommand::parse\n";

    MmMessage::parse(data);

    size_t offset = 2;
    if (data.size() < 4)
    {
        throw std::runtime_error("CipherModeCommand: data too short");
    }

    // uint8_t msgType = data[offset++];
    // if (msgType != messageType())
    // {
    //     throw std::runtime_error("CipherModeCommand: wrong message type");
    // }

    _cipherAlgorithm = data[offset++];
    _keySequence = data[offset++];

    if (offset < data.size())
    {
        _mobileIdentity = Tlv::parse(data, offset);
        if (_mobileIdentity.tag() != TLV_MI_TAG)
        {
            throw std::runtime_error("CipherModeCommand: invalid mobile identity tag");
        }

        _isMobileIdentityExist = true;
    }
    else
    {
        _isMobileIdentityExist = false;
    }
}

std::vector<uint8_t> CipherModeCommand::pack() const
{
    std::cout << "CipherModeCommand::pack\n";

    std::vector<uint8_t> out = MmMessage::pack();
    out.push_back(_cipherAlgorithm);
    out.push_back(_keySequence);

    if (_isMobileIdentityExist)
    {
        std::vector<uint8_t> miEncoded = _mobileIdentity.pack();
        out.insert(out.end(), miEncoded.begin(), miEncoded.end());
    }

    return out;
}
