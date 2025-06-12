#include "IOUtils/Csv/CsvWriter.h"

CsvWriter::CsvWriter() : FileWriter() {}

bool CsvWriter::writeRow(const CsvRow& row)
{
    return write(row.toString() + _separator);
}

bool CsvWriter::writeRows(const std::vector<CsvRow>& rows)
{
    if (!isOpen())
    {
        return false;
    }

    for (const auto& row : rows)
    {
        if (!writeRow(row))
        {
            return false;
        }
    }

    return true;
}
