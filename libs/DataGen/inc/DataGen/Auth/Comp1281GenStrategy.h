#ifndef _DATA_GEN_COMP128_1_DATA_GEN_STRATEGY
#define _DATA_GEN_COMP128_1_DATA_GEN_STRATEGY

#include "AuthDataGenStrategy.h"
#include <vector>

class Comp1281GenStrategy : public AuthDataGenStrategy
{
public:
    Comp1281GenStrategy();
    ~Comp1281GenStrategy() = default;
    std::vector<uint8_t> generateNext() override;
    
private:
    std::vector<uint8_t> _output;
    //std::vector<uint8_t> _kc;
    //std::vector<uint8_t> _sres;
};

#endif