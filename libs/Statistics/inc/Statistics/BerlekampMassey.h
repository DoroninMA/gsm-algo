#ifndef _STATISTICTS_BERLEKAMP_MASSEY_H
#define _STATISTICTS_BERLEKAMP_MASSEY_H

#include <vector>
#include <cstdint>

class BerlekampMassey
{
public:
    BerlekampMassey();

    int linearComplexity() const;

    void addBit(uint8_t bit);

private:
    static const size_t RESERVED_BYTES;

    // ring buffer for the most recent input bits (values 0 or 1)
    std::vector<uint8_t> _bitBuffer;
    size_t _bufferSize;
    size_t _bufferHead;

    // connection polynomial C(x) and its backup B(x)
    std::vector<uint8_t> _connectionPoly;
    std::vector<uint8_t> _backupPoly;

    size_t _currentLFSRLength; // current LFSR length (linear complexity)
    size_t _lastDiscrepancyPos; // last discrepancy index
    size_t _currentBitIndex; // bit counter

    // resize the ring buffer to newSize, preserving the most recent _bufferSize bits
    void _resizeBuffer(size_t newSize);

    // remove trailing zeros from a polynomial representation
    void _trimPolynomial(std::vector<uint8_t>& poly);
};

#endif
