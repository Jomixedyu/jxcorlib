#ifndef CORELIB_FILE_H
#define CORELIB_FILE_H

#include <string>

class File
{
public:
    static std::wstring ReadAllText(const std::wstring& path);

};

#endif // !CORELIB_FILE_H