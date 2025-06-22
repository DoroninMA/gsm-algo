#include <Network/Level3/CcMessage/CcMessage.h>

#include <stdexcept>

GsmMessagePD CcMessage::protocolDiscriminator() const
{
    return GsmMessagePD::CALL_CONTROL;
}

void CcMessage::parse(const std::vector<uint8_t>& data)
{
    if (!data.empty())
    {
        uint8_t pd = (data[0] >> 4);
        _tid = data[0] & 0x0F;

        if (pd != static_cast<uint8_t>(protocolDiscriminator()))
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
    std::vector<uint8_t> out;

    // Добавляем корректный CC-заголовок: PD/TI + MessageType
    out.push_back(static_cast<uint8_t>(
        ((static_cast<uint8_t>(protocolDiscriminator()) & 0x0F) << 4) | (_tid & 0x0F)));
    out.push_back(messageType());

    return out;
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
