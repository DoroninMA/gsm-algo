#include "IOUtils/Csv/CsvRow.h"

CsvRow::CsvRow(const std::string& column)
{
    addColumn(column);
}

CsvRow::CsvRow(const std::vector<std::string>& columns)
{
    addColumns(columns);
}

size_t CsvRow::columnCount() const
{
    return _columns.size();
}

const std::string& CsvRow::getColumn(size_t index) const
{
    return _columns[index];
}

std::string CsvRow::toString() const
{
    std::ostringstream oss;
    for (size_t i = 0; i < _columns.size(); ++i)
    {
        if (i != 0)
        {
            oss << _separator;
        }
        oss << _columns[i];
    }
    return oss.str();
}

void CsvRow::clear()
{
    _columns.clear();
}

CsvRow& CsvRow::addColumn(const std::string& value)
{
    // input symbol escaping
    std::string escaped = value;
    bool needsQuotes = false;

    if (escaped.find('"') != std::string::npos ||
        escaped.find(',') != std::string::npos ||
        escaped.find('\n') != std::string::npos)
    {
        needsQuotes = true;

        // replace double quote with two double quote
        size_t pos = 0;
        while ((pos = escaped.find('"', pos)) != std::string::npos)
        {
            escaped.replace(pos, 1, "\"\"");
            pos += 2;
        }
    }

    // add quotes if is need
    if (needsQuotes)
    {
        escaped = "\"" + escaped + "\"";
    }

    _columns.push_back(escaped);
    return *this;
}

CsvRow& CsvRow::addColumns(const std::vector<std::string>& columns)
{
    for (const std::string& column : columns)
    {
        addColumn(column);
    }

    return *this;
}
