#ifndef _DATA_GEN_AUTH_DATA_GEN_STRATEGY_H
#define _DATA_GEN_AUTH_DATA_GEN_STRATEGY_H

#include <vector>
#include <cstdint>

class AuthDataGenStrategy
{
public:
    const std::vector<uint8_t>& getKi() const;
    const std::vector<uint8_t>& getRand() const;

    virtual std::vector<uint8_t> generateNext() = 0;
    
    void setKi(const std::vector<uint8_t>& ki);
    void setKi(std::vector<uint8_t>&& ki);

    void setRand(const std::vector<uint8_t>& rand);
    void setRand(std::vector<uint8_t>&& rand);

protected:
    AuthDataGenStrategy();

    static const size_t SRES_BYTES;
    static const size_t KC_BYTES;

    std::vector<uint8_t> _output;

private:
    std::vector<uint8_t> _ki;
    std::vector<uint8_t> _rand;
};

#endif
