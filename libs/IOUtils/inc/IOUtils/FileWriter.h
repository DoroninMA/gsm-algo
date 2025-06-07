#include <string>
#include <fstream>

class FileWriter
{
public:
    FileWriter();
    ~FileWriter();

    bool isOpened() const;

    bool open(const std::string& filename, bool append = false);
    void close();

    bool write(const std::string& data);

private:
    std::ofstream _file;
    bool _isOpened = false;
};