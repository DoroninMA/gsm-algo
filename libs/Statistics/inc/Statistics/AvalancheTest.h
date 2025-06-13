#ifndef _STATISTICTS_AVALANCHE_TEST
#define _STATISTICTS_AVALANCHE_TEST

#include <vector>
#include <functional>
#include <cstdint>

class AvalancheTester
{
public:
    AvalancheTester(std::function<std::vector<uint8_t>(const std::vector<uint8_t>&)> func);

    size_t totalChangedBits() const;
    size_t totalFlips() const;
    double average() const;
    
    void test(const std::vector<uint8_t>& input);

private:
    std::function<std::vector<uint8_t>(const std::vector<uint8_t>&)> func;
    size_t _totalChangedBits = 0;
    size_t _totalFlips = 0;

    std::vector<uint8_t> _flipBitData(const std::vector<uint8_t>& data, size_t bitPos);
    size_t _hammingDistance(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b);
    size_t _hammingDistance(uint8_t a, uint8_t b);
    
    void _resetTotals();
};

#endif
