#include "Path.h"
#include <windows.h>

std::wstring Path::AppPath()
{
    wchar_t c[255];
    GetCurrentDirectory(255, c);
    std::wstring str(c);
    return str;
}