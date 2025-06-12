#ifndef _DATA_GEN_AUTH_INPUT_GENERATOR
#define _DATA_GEN_AUTH_INPUT_GENERATOR

#include <vector>
#include <memory>

#include "AuthDataGenStrategy.h"
#include "IOUtils/Csv/CsvWriter.h"


class AuthInputGenerator
{
public:
    using KeyString = std::vector<uint8_t>;

    AuthInputGenerator(CsvWriter* pCsvWriter);

    void setCsvWriter(CsvWriter* pCsvWriter);
    void generate(size_t count);

private:
    CsvWriter* _pCsvWriter = nullptr;

    void _writeGenResult(const KeyString& ki, const KeyString& rand);
};

#endif