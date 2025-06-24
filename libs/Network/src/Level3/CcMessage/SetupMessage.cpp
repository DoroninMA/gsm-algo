#include <Network/Level3/CcMessage/SetupMessage.h>
#include <Network/Bcd.h>

#include <IOUtils/Utils.h>

#include <iostream>
#include <stdexcept>

static constexpr uint8_t TLV_BEARER_CAP_TAG = 0x04;
static constexpr uint8_t TLV_CALLED_NUMBER_TAG = 0x06;
static constexpr uint8_t TLV_CALLING_NUMBER_TAG = 0x0A;


SetupMessage::SetupMessage()
    : _bearerCapability(TLV_BEARER_CAP_TAG, {}), _isCallingPartyExist(false)
{
}

SetupMessage::SetupMessage(const std::vector<uint8_t>& bearerCap,
                           const std::string& calledParty,
                           const std::string& callingParty)
    : _bearerCapability(TLV_BEARER_CAP_TAG, bearerCap),
      _calledPartyNumber(calledParty),
      _isCallingPartyExist(!callingParty.empty()),
      _callingPartyNumber(callingParty)
{
}

uint8_t SetupMessage::messageType() const
{
    return static_cast<uint8_t>(GsmMsgTypeCC::SETUP);
}

std::vector<uint8_t> SetupMessage::getBearerCapability() const
{
    return _bearerCapability.value();
}

const std::string& SetupMessage::calledPartyNumber() const
{
    return _calledPartyNumber;
}

const std::string& SetupMessage::callingPartyNumber() const
{
    return _callingPartyNumber;
}

bool SetupMessage::isCallingPartyNumberExist() const
{
    return _isCallingPartyExist;
}

void SetupMessage::setBearerCapability(const std::vector<uint8_t>& bc)
{
    _bearerCapability = Tlv(TLV_BEARER_CAP_TAG, bc);
}

void SetupMessage::setCalledPartyNumber(const std::string& num)
{
    _calledPartyNumber = num;
}

void SetupMessage::setCallingPartyNumber(const std::string& num)
{
    _callingPartyNumber = num;
    _isCallingPartyExist = !num.empty();
}

std::vector<uint8_t> SetupMessage::pack() const
{
    std::cout << "SetupMessage::pack\n";

    std::vector<uint8_t> out = CcMessage::pack();

    std::cout << "=============== SetupMessage =============" << std::endl;
    std::cout << "Bearer: " <<
        _bytesToHexString(_bearerCapability.value().data(), _bearerCapability.value().size()) << std::endl;
    std::cout << "CalledParty: " << _calledPartyNumber << std::endl;
    if (isCallingPartyNumberExist())
    {
        std::cout << "CallingParty: " << _callingPartyNumber << std::endl;
    }
    std::cout << "==========================================" << std::endl;

    // bcd number
    auto encodeBcdTlv = [](uint8_t tag, const std::string& number) {
        std::vector<uint8_t> bcd = Bcd::pack(number);
        return Tlv(tag, bcd).pack();
    };

    std::vector<uint8_t> bcEncoded = _bearerCapability.pack();
    std::vector<uint8_t> calledPNEncoded = encodeBcdTlv(TLV_CALLED_NUMBER_TAG, _calledPartyNumber);

    out.insert(out.end(), bcEncoded.cbegin(), bcEncoded.cend());
    out.insert(out.end(), calledPNEncoded.cbegin(), calledPNEncoded.cend());

    if (_isCallingPartyExist)
    {
        std::vector<uint8_t> callingPNEncoded = encodeBcdTlv(TLV_CALLING_NUMBER_TAG, _callingPartyNumber);
        out.insert(out.end(), callingPNEncoded.cbegin(), callingPNEncoded.cend());
    }

    return out;
}

void SetupMessage::parse(const std::vector<uint8_t>& data)
{
    std::cout << "SetupMessage::parse\n";

    CcMessage::parse(data);
    size_t offset = 2;

    Tlv bc = Tlv::parse(data, offset);
    if (bc.tag() != TLV_BEARER_CAP_TAG)
    {
        throw std::runtime_error("SetupMessage: expected bearerCapability");
    }

    _bearerCapability = bc;
    offset += _bearerCapability.length() + 2;

    // decode called party number IE
    Tlv calledTlv = Tlv::parse(data, offset);
    if (calledTlv.tag() != TLV_CALLED_NUMBER_TAG)
    {
        throw std::runtime_error("SetupMessage: expected calledParty");
    }

    // called party
    const std::string digits = Bcd::parse(calledTlv.value());
    _calledPartyNumber.clear();
    offset += _calledPartyNumber.length() + 2;

    // optional calling party number
    if (offset < data.size())
    {
        const Tlv callingTlv = Tlv::parse(data, offset);
        if (callingTlv.tag() == TLV_CALLING_NUMBER_TAG)
        {
            _callingPartyNumber = Bcd::parse(callingTlv.value());
            _isCallingPartyExist = true;
        }
    }
}
