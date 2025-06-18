#include <Network/Level3/CcMessage/CcMessage.h>

#include <stdexcept>

CcMessage::CcMessage()
{
    _pd = GsmMessagePD::CALL_CONTROL;
    _tid = 0;
}

void CcMessage::parse(const std::vector<uint8_t>& data)
{
    if (data.size() >= sizeof(_pd))
    {
        uint8_t pd = data[0] & 0xF0;
        _tid = data[0] & 0x0F;

        if (pd != static_cast<uint8_t>(_pd))
        {
            throw std::runtime_error("CcMessage::parse(): invalid protocol discriminator");
        }
    }
    else
    {
        throw std::runtime_error("CcMessage::parse(): invalid data size");
    }
}

std::vector<uint8_t> CcMessage::pack() const
{
    return std::vector<uint8_t>({ static_cast<uint8_t>((static_cast<uint8_t>(_pd) << 4) | _tid) });
}

uint8_t CcMessage::transactionId() const
{
    return _tid;
}

void CcMessage::setTransactionId(uint8_t transactionId)
{
    if (transactionId >= 0xF0)
    {
        throw std::runtime_error("CcMessage::setTransactionId(): invalid transaction id");
    }

    _tid = transactionId;
}
