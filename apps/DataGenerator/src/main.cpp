#include <iostream>
#include <stdexcept>
#include <type_traits>
//#include <DataGen/Auth/AuthDataGenerator.h>
#include "DataGen/Auth/AuthInputGenerator.h"
#include <DataGen/Auth/Comp1281GenStrategy.h>
#include <DataGen/Auth/Comp1282GenStrategy.h>
#include <DataGen/Auth/Comp1283GenStrategy.h>
#include <Statistics/AvalancheTest.h>
#include <IOUtils/Csv/CsvReader.h>
#include <IOUtils/Utils.h>


void generateAuthInput(const std::string& outputFilePrefix, size_t count)
{
    CsvWriter csvWriter;
    csvWriter.open(outputFilePrefix);

    AuthInputGenerator generator(&csvWriter);
    generator.generate(count);

    csvWriter.close();
}

template<class T>
std::vector<uint8_t> avalanchAuthWrapper(const std::vector<uint8_t>& kiRandBytes)
{
    static_assert(std::is_base_of_v<AuthDataGenStrategy, T>, "T must inherit from AuthDataGenStrategy");

    if (kiRandBytes.size() != 32)
    {
        throw std::invalid_argument("incorrect size");
    }

    T gen;
    auto midIt = kiRandBytes.cbegin() + kiRandBytes.size() / 2;
    gen.setKi(std::vector<uint8_t>(kiRandBytes.cbegin(), midIt));
    gen.setRand(std::vector<uint8_t>(midIt, kiRandBytes.cend()));

    return gen.generateNext();
}

void runAvalanchTest(const std::string& kiRandFile, const std::string& outputFilePrefix)
{
    std::cout << "start avalanche test\n";

    CsvReader reader;
    reader.open(kiRandFile);

    std::vector<AvalancheTester> testers{
        AvalancheTester(avalanchAuthWrapper<Comp1281GenStrategy>),
        AvalancheTester(avalanchAuthWrapper<Comp1282GenStrategy>),
        AvalancheTester(avalanchAuthWrapper<Comp1283GenStrategy>),
    };

    //AvalancheTester a(avalanchAuthWrapper<Comp1281GenStrategy>);
    //a.test()

    std::vector<CsvWriter> writers(testers.size());
    for (size_t i = 0; i < writers.size(); ++i)
    {
        writers[i].open(outputFilePrefix + std::to_string(i + 1) + ".csv");
    }

    while (!reader.isEndOfFile())
    {
        CsvRow kiRandRow = reader.readNextRow();
        if (kiRandRow.columnCount() < 2)
        {
            if (kiRandRow.columnCount() == 0)
            {
                break;
            }

            throw std::invalid_argument("ki-rand input file contains a row with a small number of columns");
        }

        std::string kiStr = kiRandRow.getColumn(0);
        std::string randStr = kiRandRow.getColumn(1);

        std::vector<uint8_t> ki = hexStringToBytes(kiStr.c_str(), kiStr.size());
        std::vector<uint8_t> rand = hexStringToBytes(randStr.c_str(), randStr.size());
        
        std::vector<uint8_t> kiRand(ki.size() + rand.size());
        std::copy(ki.cbegin(), ki.cend(), kiRand.begin());
        std::copy(rand.cbegin(), rand.cend(), kiRand.begin() + ki.size());

        for (size_t i = 0; i < testers.size(); ++i)
        {
            AvalancheTester& tester = testers[i];
            tester.test(kiRand);
            
            size_t changedBits = tester.totalChangedBits();
            size_t flips = tester.totalFlips();

            writers[i].writeRow(CsvRow({
                    kiStr,
                    randStr,
                    std::to_string(changedBits),
                    std::to_string(flips)
                }));
        }
    }

    std::cout << "finish avalanche test\n";
}

int main(int argc, char* argv[])
{
    std::string kiRandFileName = "auth_input.csv";

    size_t keyCount = 1000000;
    generateAuthInput(kiRandFileName, keyCount);

    runAvalanchTest(kiRandFileName, "avalanche_comp128_");

    //CsvWriter csvWriter;
    //FileWriter fileWriter;

    //fileWriter.open("comp128_1.txt");
    //csvWriter.open("comp128_1.csv");

    //AuthDataGenerator generator(new Comp1281GenStrategy());
    //generator.setFileWriter(&fileWriter);
    //generator.setCsvWriter(&csvWriter);

    //generator.generate(1000);

    //fileWriter.close();
    //csvWriter.close();

    return 0;
}
