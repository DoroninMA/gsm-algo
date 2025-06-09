#ifndef _IO_UTILS_CSV_ROW
#define _IO_UTILS_CSV_ROW

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

class CsvRow
{
public:
    CsvRow();
    ~CsvRow();

    size_t ColumnCount() const;

    const std::string& getColumn(size_t index) const;
    std::string toString() const;

    void clear();
    CsvRow& addColumn(const std::string& value);

private:
    std::vector<std::string> _columns;
    std::string _separator = ",";
};

#endif