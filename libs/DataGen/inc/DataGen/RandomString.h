#include <random>

class RandomU8IStringGenerator
{
public:
    RandomU8IStringGenerator(size_t length);
    ~RandomU8IStringGenerator() = default;

    std::vector<uint8_t> generateNext();

private:
    size_t _length = 0;
    std::random_device _rd;
    std::mt19937 _rng;
    std::uniform_int_distribution<unsigned int> _dist{0, 255};
};
