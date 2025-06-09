#ifndef _DATA_GEN_AUTH_DATA_GEN_STRATEGY
#define _DATA_GEN_AUTH_DATA_GEN_STRATEGY

#include <vector>

class AuthDataGenStrategy
{
public:
    const std::vector<uint8_t>& getKi() const;
    const std::vector<uint8_t>& getRand() const;

    virtual std::vector<uint8_t> generateNext() = 0;
    void setKi(const std::vector<uint8_t>& ki);
    void setRand(const std::vector<uint8_t>& rand);

private:
    std::vector<uint8_t> _ki;
    std::vector<uint8_t> _rand;
};

#endif