#ifndef CORELIB_FILE_H
#define CORELIB_FILE_H

#include <string>

namespace JxCoreLib
{
    class File
    {
    public:
        static std::string ReadAllText(const std::string& path);

    };
}

#endif // !CORELIB_FILE_H


