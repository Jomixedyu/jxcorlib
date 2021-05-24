#include "Path.h"
#include <windows.h>
#include "String.h"

namespace Path
{
    std::string AppPath()
    {
        wchar_t c[255];
        GetCurrentDirectory(255, c);
        return Encoding::UTF16ToUTF8(std::u16string((char16_t*)c));
    }
}
