#ifndef _DATA_GEN_COMP128_3_DATA_GEN_STRATEGY
#define _DATA_GEN_COMP128_3_DATA_GEN_STRATEGY

#include "AuthDataGenStrategy.h"
#include <vector>

class Comp1283GenStrategy : public AuthDataGenStrategy
{
public:
    Comp1283GenStrategy();
    ~Comp1283GenStrategy() = default;
    std::vector<uint8_t> generateNext() override;
};

#endif
