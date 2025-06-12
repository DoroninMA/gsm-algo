#ifndef _IO_UTILS_CSV_ROW
#define _IO_UTILS_CSV_ROW

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

class CsvRow
{
public:
    CsvRow() = default;
    CsvRow(const std::string& column);
    CsvRow(const std::vector<std::string>& columns);
    ~CsvRow() = default;

    size_t columnCount() const;

    const std::string& getColumn(size_t index) const;
    std::string toString() const;

    void clear();
    CsvRow& addColumn(const std::string& value);
    CsvRow& addColumns(const std::vector<std::string>& columns);

private:
    std::vector<std::string> _columns;
    std::string _separator = ",";
};

#endif