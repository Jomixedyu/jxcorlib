#include "File.h"
#include "UString.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace JxCoreLib
{
    namespace FileUtil
    {
        std::string ReadAllText(const std::string& path)
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


    namespace PathUtil
    {
        std::string GetFilenameWithoutExt(const std::string& path)
        {
            std::string str(path);
            int last = -1;
            for (size_t i = 0; i < str.size(); i++)
            {
                if (str[i] == '\\')
                {
                    str[i] = '/';
                }
                if (str[i] == '/')
                {
                    last = (int)i;
                }
            }
            return str.substr(last + 1, str.find_last_of('.') - last - 1);
        }
        std::string GetFilename(const std::string& path)
        {
            return StringUtil::StringCast(std::filesystem::path(path).filename().generic_u8string());
        }
    }
}

