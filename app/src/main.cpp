#include <iostream>
extern "C" {
#include "comp128.h"
}

int main()
{
    uint8_t ki[128];
    uint8_t srand[128];
    uint8_t sres[128];
    uint8_t kc[128];
    comp128(ki, srand, sres, kc);
    std::cout << "hello\n";
    return 0;
}
