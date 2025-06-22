#include <Network/Level3/MmMessage/LocationUpdateRequest.h>

#include <iostream>
#include <stdexcept>

LocationUpdateRequest::LocationUpdateRequest()
    : _locationUpdateType(0),
      _isCipherKeySeqNumExist(false),
      _cipherKeySeqNum(0)
{
}

LocationUpdateRequest::LocationUpdateRequest(
    uint8_t locationUpdateType,
    const std::vector<uint8_t>& lai,
    const MobileIdentity& mobileIdentity,
    uint8_t cipherKeySeqNum
) : _locationUpdateType(locationUpdateType),
    _lai(lai),
    _mobileIdentity(mobileIdentity),
    _cipherKeySeqNum(cipherKeySeqNum)
{
    if (cipherKeySeqNum != 0)
    {
        _isCipherKeySeqNumExist = true;
    }
}

uint8_t LocationUpdateRequest::messageType() const
{
    return static_cast<uint8_t>(GsmMsgTypeMM::LOCATION_UPDATE_REQUEST);
}

uint8_t LocationUpdateRequest::locationUpdateType() const
{
    return _locationUpdateType;
}

std::vector<uint8_t> LocationUpdateRequest::lai() const
{
    return _lai;
}

MobileIdentity LocationUpdateRequest::mobileIdentity() const
{
    return _mobileIdentity;
}

uint8_t LocationUpdateRequest::cipherKeySeqNum() const
{
    return _cipherKeySeqNum;
}

bool LocationUpdateRequest::isCipherKeySequenceNumberExist() const
{
    return _isCipherKeySeqNumExist;
}

void LocationUpdateRequest::setLocationUpdateType(uint8_t type)
{
    _locationUpdateType = type;
}

void LocationUpdateRequest::setLai(const std::vector<uint8_t>& lai)
{
    _lai = lai;
}

void LocationUpdateRequest::setMobileIdentity(const MobileIdentity& mi)
{
    _mobileIdentity = mi;
}

void LocationUpdateRequest::setCipherKeySeqNum(uint8_t seq)
{
    _cipherKeySeqNum = seq;
    _isCipherKeySeqNumExist = true;
}

void LocationUpdateRequest::parse(const std::vector<uint8_t>& data)
{
    std::cout << "LocationUpdateRequest::parse\n";

    MmMessage::parse(data);

    size_t offset = 1;
    if (data.size() < 9)
    {
        throw std::runtime_error("Too short for LocationUpdatingRequest");
    }

    uint8_t mt = data[offset++];
    if (mt != messageType())
    {
        throw std::runtime_error("Invalid message type");
    }

    _locationUpdateType = data[offset++];

    _lai = std::vector<uint8_t>(data.cbegin() + offset, data.cbegin() + offset + 5);
    offset += 5;

    std::vector<uint8_t> mi_data(data.cbegin() + offset, data.cend());
    if (!mi_data.empty() && mi_data.back() != 0x00 && mi_data.size() > 1)
    {
        _cipherKeySeqNum = mi_data.back();
        _isCipherKeySeqNumExist = true;
        mi_data.pop_back();
    }
    else
    {
        _isCipherKeySeqNumExist = false;
        _cipherKeySeqNum = 0;
    }

    _mobileIdentity.parse(mi_data);
}

std::vector<uint8_t> LocationUpdateRequest::pack() const
{
    std::cout << "LocationUpdateRequest::pack\n";

    std::vector<uint8_t> result = MmMessage::pack();

    result.push_back(messageType());
    result.push_back(_locationUpdateType);

    if (_lai.size() != 5)
    {
        throw std::runtime_error("LAI must be 5 bytes");
    }
    result.insert(result.end(), _lai.cbegin(), _lai.cend());

    auto mi = _mobileIdentity.pack();
    result.insert(result.end(), mi.cbegin(), mi.cend());

    if (_isCipherKeySeqNumExist)
    {
        result.push_back(_cipherKeySeqNum);
    }

    return result;
}
