#include "DataGen/Auth/AuthInputGenerator.h"

#include "DataGen/RandomString.h"
#include "IOUtils/Utils.h"

static const size_t KEY_SIZE = 16;

AuthInputGenerator::AuthInputGenerator(CsvWriter* pCsvWriter)
{
    setCsvWriter(pCsvWriter);
}

void AuthInputGenerator::setCsvWriter(CsvWriter* pCsvWriter)
{
    _pCsvWriter = pCsvWriter;
}

void AuthInputGenerator::generate(size_t count)
{
    RandomU8IStringGenerator generator(KEY_SIZE);

    std::cout << "start generating input sequence (" << count << ")\n";
    std::cout << "#\t" << "Ki\t" << " " << "Rand\n";
    std::cout << "_______________________________________________\n";
    for (size_t i = 0; i < count; ++i)
    {
        KeyString ki = generator.generateNext();
        KeyString rand = generator.generateNext();

        std::cout << "[" << i + 1 << "]: ";
        _writeGenResult(ki, rand);
    }

    std::cout << "generating input done (" << count << ")\n";
}

//void AuthInputGenerator::generate(size_t count)
//{
//    size_t keyCount = static_cast<size_t>(std::ceil(std::sqrt(count)));
//    size_t realCount = keyCount * keyCount;
//
//    RandomU8IStringGenerator generator(KEY_SIZE);
//    std::vector<KeyString> keys(keyCount);
//    for (size_t i = 0; i < keyCount; ++i)
//    {
//        keys[i] = generator.generateNext();
//    }
//
//    std::cout << "start generating input sequence (" << realCount << ")\n";
//    std::cout << "#\t" << "Ki\t" << " " << "Rand\n";
//    std::cout << "_______________________________________________\n";
//    for (size_t i = 0; i < keys.size(); ++i)
//    {
//        for (size_t j = 0; j < keys.size(); ++j)
//        {
//            /// @todo replace with method
//            size_t keyPairNumber = i * keys.size() + j;
//            std::cout << "[" << keyPairNumber << "]: ";
//
//            _writeGenResult(keys[i], keys[j]);
//        }
//    }
//
//    std::cout << "generating input done (" << realCount << ")\n";
//}

void AuthInputGenerator::_writeGenResult(const KeyString& ki, const KeyString& rand)
{
    if ((_pCsvWriter != nullptr) && _pCsvWriter->isOpen())
    {
        std::string kiHexString = _bytesToHexString(ki.data(), ki.size());
        std::string randHexString = _bytesToHexString(rand.data(), rand.size());

        std::cout << kiHexString << " " << randHexString << "\n";

        _pCsvWriter->writeRow(
            CsvRow().addColumn(kiHexString)
                    .addColumn(randHexString)
        );
    }
}
