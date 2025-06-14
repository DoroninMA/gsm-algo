#ifndef _DATA_GEN_COMP128_1_DATA_GEN_STRATEGY_H
#define _DATA_GEN_COMP128_1_DATA_GEN_STRATEGY_H

#include "AuthDataGenStrategy.h"
#include <vector>

class Comp1281GenStrategy : public AuthDataGenStrategy
{
public:
    Comp1281GenStrategy();
    ~Comp1281GenStrategy() = default;
    std::vector<uint8_t> generateNext() override;
};

#endif
