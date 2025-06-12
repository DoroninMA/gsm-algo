#ifndef _IO_UTILS_FILE_READER
#define _IO_UTILS_FILE_READER

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <memory>

class FileReader
{
public:
    ~FileReader();

    bool isOpen() const;
    bool isEndOfFile() const;

    void open(const std::string& filePath);
    void close();

    std::string readNextLine();

private:
    std::ifstream _file;
};

#endif