#ifndef _IO_UTILS_UTILS
#define _IO_UTILS_UTILS

#include <string>
#include <cstdint>
#include <vector>

std::string bytesToHexString(const uint8_t* str, size_t bytes);
std::string bytesToBitString(const uint8_t* str, size_t bytes);

std::vector<uint8_t> hexStringToBytes(const char* str, size_t size);


#endif
