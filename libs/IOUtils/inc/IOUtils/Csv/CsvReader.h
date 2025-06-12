#ifndef _IO_UTILS_CSV_READER
#define _IO_UTILS_CSV_READER

#include <vector>

#include <IOUtils/FileReader.h>
#include "CsvRow.h"

class CsvReader : public FileReader
{
public:
    CsvReader();

    CsvRow readNextRow();

private:
    std::vector<std::string> _splitRowString(const std::string& row, const std::string& delimiter=",");
};

#endif
