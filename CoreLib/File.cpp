#include <iostream>
#include <fstream>
#include <sstream>

#include "File.h"

std::wstring File::ReadAllText(const std::wstring& path)
{
    std::wifstream ifs;
    std::wstringstream ss;
    ifs.open(path);
    if (!ifs.is_open()) {
        throw std::invalid_argument("Unable to open file");
    }
    ss << ifs.rdbuf() << std::endl;
    ifs.close();
    return ss.str();
}