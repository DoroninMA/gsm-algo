#ifndef _DATA_GEN_AUTH_DATA_GENERATOR_H
#define _DATA_GEN_AUTH_DATA_GENERATOR_H

#include <vector>
#include <memory>

#include "AuthDataGenStrategy.h"
#include "IOUtils/Csv/CsvWriter.h"


class AuthDataGenerator
{
public:
    explicit AuthDataGenerator(AuthDataGenStrategy* pStrategy);

    void setGenStrategy(AuthDataGenStrategy* pStrategy);
    void generate(size_t count);

    void setCsvWriter(CsvWriter* pCsvWriter);
    void setFileWriter(FileWriter* pFileWriter);

private:
    std::unique_ptr<AuthDataGenStrategy> _pGenStrategy;
    CsvWriter* _pCsvWriter = nullptr;
    FileWriter* _pFileWriter = nullptr;

    std::vector<uint8_t> _ki;
    std::vector<uint8_t> _rand;

    void _setKi(const std::vector<uint8_t>& ki);
    void _setRand(const std::vector<uint8_t>& rand);

    void _writeGenResult(const std::string& genResult);
    void _writeGenResult(const std::vector<uint8_t>& genResult);
};

#endif
