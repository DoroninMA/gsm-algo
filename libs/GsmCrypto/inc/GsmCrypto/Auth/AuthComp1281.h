#ifndef _GSM_CRYPTO_AUTH_AUTH_COMP128_1_H
#define _GSM_CRYPTO_AUTH_AUTH_COMP128_1_H

#include "AuthGenerator.h"

class Comp1281 : public AuthGenerator
{
public:
    Comp1281() = default;
    ~Comp1281() override = default;

    /// \param[out] sres 4 bytes vector
    /// \param[out] kc 8 bytes vector
    void generateNext(std::vector<uint8_t>& sres, std::vector<uint8_t>& kc) override;
};

#endif