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
        using namespace std;
        std::string ReadAllText(std::string_view path)
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
        void WriteAllText(std::string_view path, std::string_view content)
        {
            ofstream outfile(path, ios::ate);
            outfile << content;
            outfile.close();
        }
    }


    namespace PathUtil
    {
        std::string GetFilenameWithoutExt(std::string_view path)
        {
            int last = -1;
            for (size_t i = 0; i < path.size(); i++)
            {
                if (path[i] == '\\' || path[i] == '/')
                {
                    last = (int)i;
                }
            }
            if (last < 0)
            {
                return std::string{ path };
            }
            return std::string{ path.substr(static_cast<size_t>(last) + 1, path.find_last_of('.') - last - 1) };
        }
        std::string GetFilename(std::string_view path)
        {
            return StringUtil::StringCast(std::filesystem::path(path).filename().generic_u8string());
        }
    }
}

