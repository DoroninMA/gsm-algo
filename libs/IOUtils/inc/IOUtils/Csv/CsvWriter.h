#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "IOUtils/FileWriter.h"
#include "CsvRow.h"

class CsvWriter : public FileWriter
{
public:
    CsvWriter();
    ~CsvWriter();

    bool writeRow(const CsvRow& row);
    bool writeRows(const std::vector<CsvRow>& rows);

private:
    std::string _separator = "\n";
};