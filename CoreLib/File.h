#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include "String.h"

namespace IO 
{
    class File
    {
    public:
        static String ReadAllText(const String &path) {
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
    };
}