#ifndef _IO_UTILS_FILE_WRITER_H
#define _IO_UTILS_FILE_WRITER_H

#include <string>
#include <fstream>

class FileWriter
{
public:
    ~FileWriter();

    bool isOpen() const;

    bool open(const std::string& filename, bool append = false);
    void close();

    bool write(const std::string& data);

private:
    std::ofstream _file;
    bool _isOpened = false;
};

#endif
