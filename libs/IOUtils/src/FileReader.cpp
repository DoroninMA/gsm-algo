#include "IOUtils/FileReader.h"

FileReader::~FileReader()
{
    close();
}

bool FileReader::isOpen() const
{
    return _file.is_open();
}

bool FileReader::isEndOfFile() const
{
    if (!isOpen())
    {
        return true;
    }

    return _file.eof();
}

void FileReader::open(const std::string& filePath)
{
    close();

    _file.open(filePath);
    if (!isOpen())
    {
        throw std::runtime_error("failed to open file: " + filePath);
    }
}

void FileReader::close()
{
    if (isOpen())
    {
        _file.close();
    }
}

std::string FileReader::readNextLine()
{
    if (!isOpen())
    {
        throw std::runtime_error("File is not opened");
    }

    std::string line;
    if (std::getline(_file, line))
    {
        return line;
    }

    // eof
    return "";
}
