#ifndef _GSM_CRYPTO_AUTH_AUTH_COMP128_3_H
#define _GSM_CRYPTO_AUTH_AUTH_COMP128_3_H

#include "AuthGenerator.h"

class Comp1283 final : public AuthGenerator
{
public:
    Comp1283() = default;
    ~Comp1283() override = default;

    /// \param[out] sres 4 bytes vector
    /// \param[out] kc 8 bytes vector
    void generateNext(std::vector<uint8_t>& sres, std::vector<uint8_t>& kc) override;
};

#endif