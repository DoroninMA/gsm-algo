#ifndef _GSM_CRYPTO_AUTH_AUTH_COMP128_2_H
#define _GSM_CRYPTO_AUTH_AUTH_COMP128_2_H

#include "AuthGenerator.h"

class Comp1282 final : public AuthGenerator
{
public:
    Comp1282() = default;
    ~Comp1282() override = default;

    /// \param[out] sres 4 bytes vector
    /// \param[out] kc 8 bytes vector
    void generateNext(std::vector<uint8_t>& sres, std::vector<uint8_t>& kc) override;
};

#endif