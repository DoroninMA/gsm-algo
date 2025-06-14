#ifndef _DATA_GEN_COMP128_2_DATA_GEN_STRATEGY_H
#define _DATA_GEN_COMP128_2_DATA_GEN_STRATEGY_H

#include "AuthDataGenStrategy.h"
#include <vector>

class Comp1282GenStrategy : public AuthDataGenStrategy
{
public:
    Comp1282GenStrategy();
    ~Comp1282GenStrategy() = default;
    std::vector<uint8_t> generateNext() override;
};

#endif
