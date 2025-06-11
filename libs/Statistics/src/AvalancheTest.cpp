#include <Statistics/AvalancheTest.h>
#include <iostream>

AvalancheTester::AvalancheTester(std::function<std::vector<uint8_t>(const std::vector<uint8_t>&)> func)
    : func(std::move(func)), _totalChangedBits(0), _totalFlips(0)
{
}

double AvalancheTester::getResult() const
{
    double average = static_cast<double>(_totalChangedBits) / _totalFlips;
    return average;
}

void AvalancheTester::addData(const std::vector<uint8_t>& input)
{
    std::vector<uint8_t> baseOutput = func(input);
    for (size_t bitPos = 0; bitPos < baseOutput.size() * sizeof(uint8_t); ++bitPos)
    {
        std::vector<uint8_t> modifiedInput = _flipBitData(input, bitPos);
        std::vector<uint8_t> modifiedOutput = func(modifiedInput);

        _totalChangedBits += _hammingDistance(baseOutput, modifiedOutput);
        ++_totalFlips;
    }
}

std::vector<uint8_t> AvalancheTester::_flipBitData(const std::vector<uint8_t>& data, size_t bitPos)
{
    // copy input
    std::vector<uint8_t> result = data;

    // making flip
    size_t bitInBytePosition = bitPos % 8;
    size_t byteIndex = bitPos / 8;
    uint8_t mask = 1 << bitInBytePosition;
    result[byteIndex] ^= mask;

    return result;
}

size_t AvalancheTester::_hammingDistance(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b)
{
    if (a.size() != b.size())
    {
        throw std::invalid_argument("incorrect hamming distance sizes (must equal sizes)");
    }

    size_t distance = 0;
    for (size_t i = 0; i < a.size(); ++i)
    {
        distance += _hammingDistance(a.at(i), b.at(i));
    }

    return distance;
}

size_t AvalancheTester::_hammingDistance(uint8_t a, uint8_t b)
{
    uint8_t xorVal = a ^ b;
    size_t distance = 0;
    while (xorVal != 0)
    {
        distance += xorVal & 1;
        xorVal >>= 1;
    }
    return distance;
}
