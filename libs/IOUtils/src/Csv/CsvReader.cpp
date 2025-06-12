#include "IOUtils/Csv/CsvReader.h"

#include <regex>

CsvReader::CsvReader() : FileReader()
{
}

CsvRow CsvReader::readNextRow()
{
    std::string line = readNextLine();
    
    if (line.empty())
    {
        return {};
    }

    return CsvRow(_splitRowString(line));
}

std::vector<std::string> CsvReader::_splitRowString(const std::string& row, const std::string& delimiter)
{
    std::vector<std::string> columns;
    std::regex re(delimiter);

    std::sregex_token_iterator it(row.begin(), row.end(), re, -1);
    std::sregex_token_iterator end;

    while (it != end)
    {
        columns.push_back(*it++);
    }

    return columns;
}