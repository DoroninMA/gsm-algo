#include "Statistics/BerlekampMassey.h"

#include <stdexcept>

const size_t BerlekampMassey::RESERVED_BYTES = 128;

BerlekampMassey::BerlekampMassey()
    : _currentLFSRLength(0),
    _lastDiscrepancyPos(-1),
    _currentBitIndex(0),
    _bufferSize(1),
    _bufferHead(0)
{
    _bitBuffer.reserve(RESERVED_BYTES);
    _connectionPoly.reserve(RESERVED_BYTES);
    _backupPoly.reserve(RESERVED_BYTES);

    _connectionPoly.push_back(1);
    _backupPoly.push_back(1);
    _bitBuffer.assign(_bufferSize, 0);
}

int BerlekampMassey::linearComplexity() const
{
    return _currentLFSRLength;
}

void BerlekampMassey::addBit(uint8_t bit)
{
    if ((bit != 0) && (bit != 1))
    {
        throw std::invalid_argument("not a bit was passed in");
    }

    // store the new bit in the ring buffer
    _bitBuffer[_bufferHead] = bit;
    _bufferHead = (_bufferHead + 1) % _bufferSize;

    // compute discrepancy d_n = s_n xor sum(C_i, i=1..L) * s_{n−i}
    uint8_t discrepancy = bit;
    for (size_t i = 1; i <= _currentLFSRLength; ++i)
    {
        size_t idx = (_bufferHead + _bufferSize - i) % _bufferSize;
        discrepancy ^= (_connectionPoly[i] & _bitBuffer[idx]);
    }

    // if discrepancy is 1, update connection polynomial
    if (discrepancy)
    {
        std::vector<uint8_t> oldC = _connectionPoly; // C^(n)(x)
        size_t shift = _currentBitIndex - static_cast<size_t>(_lastDiscrepancyPos);

        // C(x) := C(x) xor xshift * B(x)
        if (_connectionPoly.size() < _backupPoly.size() + shift)
        {
            _connectionPoly.resize(_backupPoly.size() + shift, 0);
        }
        for (size_t i = 0; i < _backupPoly.size(); ++i)
        {
            _connectionPoly[i + shift] ^= _backupPoly[i];
        }

        _trimPolynomial(_connectionPoly);

        // if 2L <= n then update B(x), L, m = n
        if (2 * _currentLFSRLength <= _currentBitIndex)
        {
            _backupPoly = std::move(oldC);
            _currentLFSRLength = static_cast<int>(_currentBitIndex + 1 - _currentLFSRLength);
            _lastDiscrepancyPos = static_cast<std::ptrdiff_t>(_currentBitIndex);
            // ensure buffer can hold L+1 past bits
            if (_bufferSize < _currentLFSRLength + 1)
            {
                _resizeBuffer(_currentLFSRLength + 1);
            }
        }
    }

    // advance to next bit position
    ++_currentBitIndex;
}

void BerlekampMassey::_resizeBuffer(size_t newSize)
{
    std::vector<uint8_t> newBuf(newSize, 0);
    // copy bits in order from oldest to newest
    for (size_t i = 0; i < _bufferSize; ++i)
    {
        size_t oldIdx = (_bufferHead + i) % _bufferSize;
        newBuf[i] = _bitBuffer[oldIdx];
    }
    size_t oldSize = _bufferSize;
    _bitBuffer.swap(newBuf);
    _bufferSize = newSize;

    // next write goes after the last copied element
    _bufferHead = oldSize % newSize;
}

void BerlekampMassey::_trimPolynomial(std::vector<uint8_t>& poly)
{
    while (poly.size() > 1 && poly.back() == 0)
    {
        poly.pop_back();
    }
}
