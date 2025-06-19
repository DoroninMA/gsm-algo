#ifndef _GSM_CRYPTO_AUTH_AUTH_GENERATOR_H
#define _GSM_CRYPTO_AUTH_AUTH_GENERATOR_H

#include <vector>
#include <cstdint>

class AuthGenerator
{
public:
    const std::vector<uint8_t>& ki() const;
    const std::vector<uint8_t>& rand() const;

    virtual void generateNext(std::vector<uint8_t>& sres, std::vector<uint8_t>& kc) = 0;

    void setKi(const std::vector<uint8_t>& ki);
    void setKi(std::vector<uint8_t>&& ki);

    void setRand(const std::vector<uint8_t>& rand);
    void setRand(std::vector<uint8_t>&& rand);

protected:
    AuthGenerator() = default;
    virtual ~AuthGenerator() = default;

    static const size_t SRES_BYTES;
    static const size_t KC_BYTES;

private:
    std::vector<uint8_t> _ki;
    std::vector<uint8_t> _rand;
};

#endif
