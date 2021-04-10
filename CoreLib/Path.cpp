#include "Path.h"
#include <windows.h>
#include "StringUtility.h"

namespace Path
{
    std::string AppPath()
    {
        wchar_t c[255];
        GetCurrentDirectory(255, c);
        return StringUtility::WstringToString(c);
    }
}
