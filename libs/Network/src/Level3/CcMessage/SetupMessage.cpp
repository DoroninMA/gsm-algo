#include <Network/Level3/CcMessage/SetupMessage.h>
#include <Network/Bcd.h>

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
    return static_cast<uint8_t>(GsmMsgTypeL3::SETUP);
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
    std::vector<uint8_t> out;
    out.push_back(static_cast<uint8_t>(GsmMsgTypeL3::SETUP));

    // bearerCapability
    std::vector<uint8_t> bcPacked = _bearerCapability.pack();
    out.insert(out.end(), bcPacked.begin(), bcPacked.end());

    // BCD to TLV converter
    auto encodeBcdTlv = [](const uint8_t& tag, const std::string& number) -> std::vector<uint8_t>
    {
        std::string digits;
        for (char c: number)
        {
            if (!std::isdigit(static_cast<unsigned char>(c)))
            {
                throw std::runtime_error("SetupMessage: non-digit character in phone number");
            }
            digits.push_back(c - '0');
        }
        std::vector<uint8_t> bcd = Bcd::pack(digits);
        return Tlv(tag, bcd).pack();
    };

    // calledPartyNumber
    std::vector<uint8_t> calledEnc = encodeBcdTlv(TLV_CALLED_NUMBER_TAG, _calledPartyNumber);
    out.insert(out.end(), calledEnc.begin(), calledEnc.end());

    // callingPartyNumber
    if (_isCallingPartyExist)
    {
        std::vector<uint8_t> callingEnc = encodeBcdTlv(TLV_CALLING_NUMBER_TAG, _callingPartyNumber);
        out.insert(out.end(), callingEnc.begin(), callingEnc.end());
    }

    return out;
}

void SetupMessage::parse(const std::vector<uint8_t>& data)
{
    size_t offset = 0;
    if (data.size() < 2)
    {
        throw std::runtime_error("SetupMessage: data too short");
    }

    uint8_t mt = data[offset++];
    if (mt != static_cast<uint8_t>(GsmMsgTypeL3::SETUP))
    {
        throw std::runtime_error("SetupMessage: wrong message type");
    }

    Tlv bc = Tlv::parse(data, offset);
    if (bc.tag() != TLV_BEARER_CAP_TAG)
    {
        throw std::runtime_error("SetupMessage: expected bearerCapability");
    }

    _bearerCapability = bc;

    // decode called party number IE
    Tlv calledTlv = Tlv::parse(data, offset);
    if (calledTlv.tag() != TLV_CALLED_NUMBER_TAG)
    {
        throw std::runtime_error("SetupMessage: expected calledParty");
    }

    // BCD decode
    {
        const std::string digits = Bcd::parse(calledTlv.value());
        _calledPartyNumber.clear();

        for (auto d: digits)
        {
            _calledPartyNumber.push_back('0' + d);
        }
    }

    // optional calling party number
    if (offset < data.size())
    {
        const Tlv callingTlv = Tlv::parse(data, offset);
        if (callingTlv.tag() == TLV_CALLING_NUMBER_TAG)
        {
            {
                const std::string digits = Bcd::parse(callingTlv.value());
                _callingPartyNumber.clear();
                for (const char d: digits)
                {
                    _callingPartyNumber.push_back('0' + d);
                }

                _isCallingPartyExist = true;
            }

            _isCallingPartyExist = true;
        }
    }
}
