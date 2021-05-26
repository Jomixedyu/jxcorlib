#include <iostream>
#include <fstream>
#include <sstream>

#include "File.h"

namespace JxCoreLib
{
    std::string File::ReadAllText(const std::string& path)
    {
        std::ifstream ifs;
        std::stringstream ss;
        ifs.open(path);
        if (!ifs.is_open()) {
            throw std::invalid_argument("Unable to open file");
        }
        ss << ifs.rdbuf() << std::endl;
        ifs.close();
        return ss.str();
    }
}