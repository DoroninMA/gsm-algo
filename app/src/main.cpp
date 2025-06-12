#include <iostream>
//#include <DataGen/Auth/AuthDataGenerator.h>
#include "DataGen/Auth/AuthInputGenerator.h"
#include <DataGen/Auth/Comp1281GenStrategy.h>
#include <Statistics/AvalancheTest.h>

void generateAuthInput()
{
    CsvWriter csvWriter;
    csvWriter.open("auth_input.csv");

    AuthInputGenerator generator(&csvWriter);
    generator.generate(1000);

    csvWriter.close();
}

int main(int argc, char* argv[])
{
    generateAuthInput();
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
