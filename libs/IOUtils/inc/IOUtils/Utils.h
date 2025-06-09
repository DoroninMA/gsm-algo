#ifndef _IO_UTILS_UTILS
#define _IO_UTILS_UTILS

#include <string>

std::string bytesToHexString(const uint8_t* str, size_t bytes);
std::string bytesToBitString(const uint8_t* str, size_t bytes);

#endif