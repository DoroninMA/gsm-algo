#include "IOUtils/FileWriter.h"

FileWriter::FileWriter()
{
}

FileWriter::~FileWriter()
{
    if (_isOpened)
    {
        _file.close();
    }
}

bool FileWriter::isOpened() const
{
    return _isOpened;
}

bool FileWriter::open(const std::string& filename, bool append)
{
    if (_isOpened)
    {
        _file.close();
    }

    std::ios_base::openmode mode = std::ios::out;
    if (append)
    {
        mode |= std::ios::app;
    }

    _file.open(filename, mode);
    _isOpened = _file.is_open();

    return _isOpened;
}

void FileWriter::close()
{
    if (_isOpened)
    {
        _file.close();
        _isOpened = false;
    }
}

bool FileWriter::write(const std::string& data)
{
    if (!_isOpened)
    {
        return false;
    }

    _file << data;
    return _file.good();
}
