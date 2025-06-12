#include "DataGen/Auth/AuthDataGenerator.h"

#include <cmath>
#include <iostream>

#include "DataGen/RandomString.h"
#include "IOUtils/Utils.h"

static const size_t KI_SIZE = 16;
using KeyString = std::vector<uint8_t>;


AuthDataGenerator::AuthDataGenerator(AuthDataGenStrategy* pStrategy)
{
    setGenStrategy(pStrategy);
}

void AuthDataGenerator::setGenStrategy(AuthDataGenStrategy* pStrategy)
{
    _pGenStrategy.reset(pStrategy);
}

void AuthDataGenerator::generate(size_t count)
{
    if (_pGenStrategy.get() == nullptr)
    {
        return;
    }

    size_t keyCount = static_cast<size_t>(std::ceil(std::sqrt(count)));

    RandomU8IStringGenerator generator(KI_SIZE);
    std::vector<KeyString> keys(keyCount);
    for (size_t i = 0; i < keyCount; ++i)
    {
        keys[i] = generator.generateNext();
    }

    std::cout << "#\t" << "Ki\t" << " " << "Rand\t" << " " << "Result" << "\n";
    std::cout << "_______________________________________________\n";
    for (size_t i = 0; i < keys.size(); ++i)
    {
        for (size_t j = 0; j < keys.size(); ++j)
        {
            _setKi(keys.at(i));
            _setRand(keys.at(j));
            KeyString genResult = _pGenStrategy->generateNext();

            /// @todo replace with method
            size_t keyPairNumber = i * keys.size() + j;
            std::cout << "[" << keyPairNumber << "]: ";

            _writeGenResult(genResult);
        }
    }
}

void AuthDataGenerator::setCsvWriter(CsvWriter* pCsvWriter)
{
    _pCsvWriter = pCsvWriter;
}

void AuthDataGenerator::setFileWriter(FileWriter* pFileWriter)
{
    _pFileWriter = pFileWriter;
}

void AuthDataGenerator::_setKi(const std::vector<uint8_t>& ki)
{
    _ki = ki;

    if (_pGenStrategy.get() != nullptr)
    {
        _pGenStrategy->setKi(_ki);
    }
}

void AuthDataGenerator::_setRand(const std::vector<uint8_t>& rand)
{
    _rand = rand;

    if (_pGenStrategy.get() != nullptr)
    {
        _pGenStrategy->setRand(_rand);
    }
}

void AuthDataGenerator::_writeGenResult(const std::string& genResult)
{
    if ((_pCsvWriter != nullptr) && _pCsvWriter->isOpen())
    {
        std::string kiHexString = bytesToHexString(_ki.data(), _ki.size());
        std::string randHexString = bytesToHexString(_rand.data(), _rand.size());

        _pCsvWriter->writeRow(
            CsvRow().addColumn(kiHexString)
            .addColumn(randHexString)
            .addColumn(genResult)
        );
    }

    if ((_pFileWriter != nullptr) && _pFileWriter->isOpen())
    {
        _pFileWriter->write(genResult + "\n");
    }
}

void AuthDataGenerator::_writeGenResult(const std::vector<uint8_t>& genResult)
{
    if ((_pCsvWriter != nullptr) && _pCsvWriter->isOpen())
    {
        std::string kiHexString = 
            bytesToHexString(_ki.data(), _ki.size());
        std::string randHexString = 
            bytesToHexString(_rand.data(), _rand.size());
        std::string genResultHexString = 
            bytesToHexString(genResult.data(), genResult.size());

        _pCsvWriter->writeRow(
            CsvRow().addColumn(kiHexString)
            .addColumn(randHexString)
            .addColumn(genResultHexString)
        );

        /// @todo repace with other method
        std::cout << kiHexString << " " << randHexString << " " << genResultHexString << "\n";
    }

    if ((_pFileWriter != nullptr) && _pFileWriter->isOpen())
    {
        std::string bitGenResultString = 
            bytesToBitString(genResult.data(), genResult.size());

        _pFileWriter->write(bitGenResultString + "\n");
    }
}

//const std::vector<std::string>& AuthDataGenerator::generateNext(size_t count)
//{
//    if (_pGenStrategy.get() != nullptr)
//    {
//        for (size_t i = 0; i < count; ++i)
//        {
//            _bitOutputs.push_back(_pGenStrategy->generateNext());
//        }
//
//        return _bitOutputs;
//    }
//}

//const std::vector<std::string>& AuthDataGenerator::generateAll()
//{
//    if (_pGenStrategy.get() != nullptr)
//    {
//        for (size_t i = 0; i < ; ++i)
//        {
//            _bitOutputs.push_back(_pGenStrategy->generateNext());
//        }
//
//        return _bitOutputs;
//    }
//}

//void AuthDataGenerator::clear()
//{
//    _bitOutputs.clear();
//}
